#!/usr/bin/env bash

set -euo pipefail

if [ -z "${GH_TOKEN:-}" ]; then
  echo "Error: GH_TOKEN environment variable is required."
  exit 1
fi

echo "Scanning for ANALYSIS.md files added in the last 24 hours..."

EXISTING_ISSUES=$(gh issue list \
  --label "daily-solution" \
  --json title \
  --jq '.[].title' \
  --limit 1000)

declare -A SLUG_TO_FILE
declare -A SLUG_TO_LANGS

while IFS= read -r FILE_PATH; do
  [ -z "$FILE_PATH" ] && continue

  if [ ! -f "$FILE_PATH" ]; then
    continue
  fi

  SLUG=$(basename "$(dirname "$FILE_PATH")")
  LANG=$(basename "$(dirname "$(dirname "$FILE_PATH")")")

  if [ -z "${SLUG_TO_FILE[$SLUG]+x}" ]; then
    SLUG_TO_FILE[$SLUG]="$FILE_PATH"
  fi

  SLUG_TO_LANGS[$SLUG]="${SLUG_TO_LANGS[$SLUG]:-}${SLUG_TO_LANGS[$SLUG]:+ }$LANG"

done < <(
  git log \
    --since="24 hours ago" \
    --name-only \
    --diff-filter=A \
    --pretty="" |
  grep 'ANALYSIS.md$' |
  sort -u
)

if [ ${#SLUG_TO_FILE[@]} -eq 0 ]; then
  echo "No new ANALYSIS.md files found in the last 24 hours."
  exit 0
fi

ISSUE_COUNT=0
BODY_FILE=$(mktemp)

trap 'rm -f "$BODY_FILE"' EXIT

for SLUG in "${!SLUG_TO_FILE[@]}"; do
  REPRESENTATIVE="${SLUG_TO_FILE[$SLUG]}"

  LANGS=$(echo "${SLUG_TO_LANGS[$SLUG]}" \
    | tr ' ' '\n' \
    | sort -u \
    | grep -v '^$' \
    | paste -sd ', ')

  FIRST_HEADING=$(grep -m 1 '^#' "$REPRESENTATIVE" | sed 's/^#\s*//')
  TOPIC=$(echo "${FIRST_HEADING:-$SLUG}" | sed 's/ (.*//')

  DIR_CONTEXT=$(dirname "$REPRESENTATIVE")
  TITLE="$TOPIC ($DIR_CONTEXT)"

  if echo "$EXISTING_ISSUES" | grep -Fq "($DIR_CONTEXT)"; then
    echo "Issue already exists, skipping: $TITLE"
    continue
  fi

  {
    echo "**Languages:** $LANGS"
    echo ""
    cat "$REPRESENTATIVE"
  } > "$BODY_FILE"

  gh issue create \
    --title "$TITLE" \
    --body-file "$BODY_FILE" \
    --label "daily-solution" \
    --label "dsa-lecture"

  echo "Created issue: $TITLE [$LANGS]"

  ISSUE_COUNT=$((ISSUE_COUNT + 1))
done

echo ""
echo "Total issues created: $ISSUE_COUNT"