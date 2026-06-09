#!/usr/bin/env bash
# scripts/create-daily-issues.sh

set -euo pipefail

if [ -z "${GH_TOKEN:-}" ]; then
  echo "Error: GH_TOKEN environment variable is required."
  exit 1
fi

echo "Scanning for ANALYSIS.md files..."

EXISTING_ISSUES=$(gh issue list --label "daily-solution" --json title --jq '.[].title' --limit 1000)

declare -A SLUG_TO_FILE
declare -A SLUG_TO_LANGS

while IFS= read -r FILE_PATH; do
  [ -z "$FILE_PATH" ] && continue

  SLUG=$(dirname "$FILE_PATH" | xargs basename)

  LANG=$(basename "$(dirname "$(dirname "$FILE_PATH")")")

  if [ -z "${SLUG_TO_FILE[$SLUG]+x}" ]; then
    SLUG_TO_FILE[$SLUG]="$FILE_PATH"
  fi

  SLUG_TO_LANGS[$SLUG]="${SLUG_TO_LANGS[$SLUG]:-}${SLUG_TO_LANGS[$SLUG]:+ }$LANG"

done < <(find . -path "./.git" -prune -o -name "ANALYSIS.md" -print | sed 's|^\./||' | sort)

ISSUE_COUNT=0
BODY_FILE=$(mktemp)
trap 'rm -f "$BODY_FILE"' EXIT

for SLUG in "${!SLUG_TO_FILE[@]}"; do
  REPRESENTATIVE="${SLUG_TO_FILE[$SLUG]}"
  LANGS=$(echo "${SLUG_TO_LANGS[$SLUG]}" | tr ' ' '\n' | sort -u | grep -v '^$' | paste -sd ', ')

  FIRST_HEADING=$(grep -m 1 '^#' "$REPRESENTATIVE" | sed 's/^#\s*//')
  TITLE="${FIRST_HEADING:-$SLUG}"

  if echo "$EXISTING_ISSUES" | grep -qF "$TITLE"; then
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