#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Generate PROGRESS.md from Markdown checklists (DEVELOP_ROADMAP.md, HABITS.md).
- Computes overall progress per file
- Breaks down Roadmap by "#### Week ..." subsections
- Breaks down Habits by tags (FITNESS, GUITAR)

Usage:
  python3 Engine/Scripts/progress/generate_progress.py [root_dir]
If root_dir omitted, script assumes it is run from repo root.
"""
import sys
import re
from pathlib import Path
from datetime import datetime
from typing import Dict, Tuple, List

CHECKBOX_RE = re.compile(r"^\s*-\s*\[(?P<state>[ xX])\]\s*(?P<label>.*)$")
TAG_RE = re.compile(r"\b([A-Z][A-Z0-9_]+)\b")

FILES = [
    "DEVELOP_ROADMAP.md",
    "HABITS.md",
]

WEEK_HEADER_RE = re.compile(r"^####\s*(Week\s*[^\n]+)")
SECTION_L3_RE = re.compile(r"^###\s*(.+)")

PROGRESS_WIDTH = 24

def progress_bar(done: int, total: int, width: int = PROGRESS_WIDTH) -> str:
    if total <= 0:
        return f"[{'░'*width}] 0% (0/0)"
    ratio = min(1.0, max(0.0, done / total))
    filled = int(round(ratio * width))
    bar = "█" * filled + "░" * (width - filled)
    pct = int(round(ratio * 100))
    return f"[{bar}] {pct}% ({done}/{total})"


def count_checkboxes(text: str) -> Tuple[int, int]:
    done = total = 0
    for line in text.splitlines():
        m = CHECKBOX_RE.match(line)
        if not m:
            continue
        total += 1
        if m.group("state").lower() == "x":
            done += 1
    return done, total


def roadmap_breakdown(text: str) -> Dict[str, Tuple[int, int]]:
    # Breakdown by '#### Week ...' blocks
    breakdown: Dict[str, Tuple[int, int]] = {}
    lines = text.splitlines()
    current_key = None
    buffer: List[str] = []

    def flush():
        nonlocal buffer, current_key
        if current_key is None:
            return
        d, t = count_checkboxes("\n".join(buffer))
        prev = breakdown.get(current_key, (0, 0))
        breakdown[current_key] = (prev[0] + d, prev[1] + t)
        buffer = []

    for line in lines:
        if WEEK_HEADER_RE.match(line):
            flush()
            current_key = WEEK_HEADER_RE.match(line).group(1).strip()
        elif SECTION_L3_RE.match(line) and line.startswith("### Week"):
            # Start of a new big week section, flush current minor week
            flush()
            # keep current_key until next #### Week
            buffer = []
            continue
        buffer.append(line)
    flush()
    return breakdown


def habits_breakdown(text: str) -> Dict[str, Tuple[int, int]]:
    # Count by tags FITNESS / GUITAR specifically
    tag_totals: Dict[str, Tuple[int, int]] = {}
    for line in text.splitlines():
        m = CHECKBOX_RE.match(line)
        if not m:
            continue
        label = m.group("label")
        tags = set(x for x in TAG_RE.findall(label))
        for tag in ("FITNESS", "GUITAR"):
            if tag in tags:
                done = 1 if m.group("state").lower() == "x" else 0
                prev = tag_totals.get(tag, (0, 0))
                tag_totals[tag] = (prev[0] + done, prev[1] + 1)
    return tag_totals


def main(root: Path) -> int:
    root = root.resolve()
    contents: Dict[str, str] = {}
    for rel in FILES:
        p = root / rel
        if p.exists():
            contents[rel] = p.read_text(encoding="utf-8")
        else:
            contents[rel] = ""

    # Overall per-file
    overall = {name: count_checkboxes(text) for name, text in contents.items()}

    # Roadmap breakdown
    roadmap = roadmap_breakdown(contents.get("DEVELOP_ROADMAP.md", ""))

    # Habits breakdown
    habits = habits_breakdown(contents.get("HABITS.md", ""))

    # Combined totals
    total_done = sum(d for d, _ in overall.values())
    total_total = sum(t for _, t in overall.values())

    out_lines: List[str] = []
    out_lines.append("# Project Progress")
    out_lines.append("")
    out_lines.append(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    out_lines.append("")

    out_lines.append("## Overall")
    out_lines.append("")
    for name in FILES:
        d, t = overall.get(name, (0, 0))
        out_lines.append(f"- {name}: {progress_bar(d, t)}")
    out_lines.append(f"- Combined: {progress_bar(total_done, total_total)}")
    out_lines.append("")

    if roadmap:
        out_lines.append("## Roadmap by Week")
        out_lines.append("")
        # Sort by natural week order if possible
        def week_key(k: str):
            # Extract first number for ordering
            import re as _re
            m = _re.search(r"(\d+)", k)
            return int(m.group(1)) if m else 9999
        for k in sorted(roadmap.keys(), key=week_key):
            d, t = roadmap[k]
            out_lines.append(f"- {k}: {progress_bar(d, t)}")
        out_lines.append("")

    out_lines.append("## Habits")
    out_lines.append("")
    if habits:
        for tag in ("FITNESS", "GUITAR"):
            d, t = habits.get(tag, (0, 0))
            out_lines.append(f"- {tag}: {progress_bar(d, t)}")
    else:
        out_lines.append("- No habits detected yet.")
    out_lines.append("")

    (root / "builds" / "PROGRESS.md").write_text("\n".join(out_lines) + "\n", encoding="utf-8")
    print("Wrote PROGRESS.md")
    return 0


if __name__ == "__main__":
    root_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else Path.cwd()
    raise SystemExit(main(root_dir))
