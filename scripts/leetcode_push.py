import os
import sys
import subprocess
from datetime import date

# ---------- Arguments ----------
language = sys.argv[1]
pid = sys.argv[2]
title = sys.argv[3].strip()

# ---------- Helpers ----------
def run(cmd):
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"⚠️ Command failed: {cmd}")
        print(result.stderr)
    return result

def slugify(text):
    return text.replace(" ", "").replace("-", "")

# ---------- File creation ----------
folder = language
os.makedirs(folder, exist_ok=True)

filename = f"{pid}_{slugify(title)}.py"
filepath = os.path.join(folder, filename)

if not os.path.exists(filepath):
    with open(filepath, "w") as f:
        f.write(f"# LeetCode {pid}: {title}\n\n")
        f.write("class Solution:\n")
        f.write("    pass\n")
else:
    input(f"✅ File already exists: {filepath}\nPaste your solution now, save it, then press Enter to continue...")

# ---------- README update ----------
readme = "README.md"
today = date.today().isoformat()
entry = f"- [{pid}] {title} ({language})\n"

if not os.path.exists(readme):
    with open(readme, "w") as f:
        f.write("# LeetCode Solutions\n\n")
        f.write("## Problems Solved\n")

with open(readme, "r+") as f:
    content = f.read()
    if entry not in content:
        f.write(entry)

# ---------- Git Safety ----------
# Save uncommitted changes if any
status = subprocess.check_output("git status --porcelain", shell=True).decode().strip()
if status:
    print("💾 Stashing uncommitted changes before syncing with remote...")
    run("git stash push -m 'Stash before auto-push'")

# Pull latest changes from remote
print("🌐 Pulling latest changes from GitHub...")
run("git pull origin main --rebase")

# Restore stashed changes
if status:
    print("💾 Restoring stashed changes...")
    run("git stash pop")

# Stage solution + README
run(f'git add "{filepath}" "{readme}"')

# Commit if there are changes
status = subprocess.check_output("git status --porcelain", shell=True).decode().strip()
if status:
    run(f'git commit -m "Solved #{pid} - {title}"')
    run("git push origin main")
else:
    print("⚠️ No changes to commit. Nothing new to push.")

# Done
print(f"\n✅ Solved #{pid}: {title}")
print(f"🔥 Streak updated for {today}")
