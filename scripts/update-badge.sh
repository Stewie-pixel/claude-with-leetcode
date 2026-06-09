#!/usr/bin/env bash

set -euo pipefail

REPO="${1:-}"
BRANCH="${2:-main}"

if [ -z "$REPO" ]; then
  echo "Error: repository argument is required."
  echo "Usage: $0 <owner/repo> [branch]"
  exit 1
fi

if ! command -v jq &>/dev/null; then
  echo "Error: jq is required but not installed."
  exit 1
fi

echo "Scanning for ANALYSIS.md files..."

declare -A SEEN_SLUGS
RAW_ENTRIES=()

while IFS= read -r FILE_PATH; do
  [ -z "$FILE_PATH" ] && continue

  SLUG=$(echo "$FILE_PATH" | awk -F'/' '{print $(NF-1)}')

  if [ -n "${SEEN_SLUGS[$SLUG]+x}" ]; then
    continue
  fi
  SEEN_SLUGS[$SLUG]=1

  TITLE=$(grep -m 1 '^#' "$FILE_PATH" 2>/dev/null | sed 's/^#\s*//')
  TITLE="${TITLE:-$SLUG}"

  FILE_URL="https://github.com/$REPO/blob/$BRANCH/$FILE_PATH"

  ENTRY=$(jq -n \
    --arg slug "$SLUG" \
    --arg title "$TITLE" \
    --arg url "$FILE_URL" \
    '{slug: $slug, title: $title, url: $url}')

  RAW_ENTRIES+=("$ENTRY")

done < <(find . -path "./.git" -prune -o -name "ANALYSIS.md" -print | sed 's|^\./||' | sort)

ENTRIES=$(printf '%s\n' "${RAW_ENTRIES[@]}" | jq -s '.')
COUNT=$(echo "$ENTRIES" | jq '. | length')

mkdir -p .github/badges

echo "$ENTRIES" | jq '.' > .github/badges/solutions.json
echo "Written: .github/badges/solutions.json ($COUNT entries)"

cat > .github/badges/badge.json <<EOF
{
  "schemaVersion": 1,
  "label": "LeetCode solutions",
  "message": "$COUNT solved"
}
EOF
echo "Written: .github/badges/badge.json"

echo ""
echo "Total unique problems: $COUNT"