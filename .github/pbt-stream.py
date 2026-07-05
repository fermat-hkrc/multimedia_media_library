#!/usr/bin/env python3
"""Run a pi-pbt command and stream its live campaign progress to stdout so the
GitHub Actions log plays in real time (pi-pbt -p buffers stdout to a pipe; the
session JSONL is written live, so we tail that instead).

usage: pbt-stream.py <sessions_dir> -- <pi-pbt cmd...>
"""
import glob, json, os, subprocess, sys, time

sess = sys.argv[1]
assert sys.argv[2] == "--", "expected -- separator"
cmd = sys.argv[3:]

proc = subprocess.Popen(cmd, stdin=subprocess.DEVNULL,
                        stdout=open("/tmp/pbt-raw.log", "w"), stderr=subprocess.STDOUT)
seen = {}
def drain():
    for f in sorted(glob.glob(sess + "/*/*.jsonl"), key=os.path.getmtime)[-2:]:
        try:
            lines = open(f, errors="replace").read().splitlines()
        except OSError:
            continue
        for ln in lines[seen.get(f, 0):]:
            try:
                e = json.loads(ln)
            except ValueError:
                continue
            m = e.get("message", {})
            if m.get("role") == "assistant" and isinstance(m.get("content"), list):
                for c in m["content"]:
                    if c.get("type") == "text" and (c.get("text") or "").strip():
                        print("  💬 " + c["text"][:240].replace("\n", " "), flush=True)
                    elif c.get("type") == "toolCall":
                        print("  🔧 %s %s" % (c.get("name"), str(c.get("arguments"))[:140].replace("\n", " ")), flush=True)
        seen[f] = len(lines)

try:
    while proc.poll() is None:
        time.sleep(4)
        drain()
    drain()
finally:
    rc = proc.wait()
    print("--- tail of raw output ---", flush=True)
    try:
        print("".join(open("/tmp/pbt-raw.log", errors="replace").read().splitlines(keepends=True)[-8:]), flush=True)
    except OSError:
        pass
    sys.exit(rc)
