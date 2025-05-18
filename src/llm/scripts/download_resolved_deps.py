# Module LLM ファームウェアアップグレードガイド
# https://docs.m5stack.com/ja/guide/llm/llm/image
#
# アプリケーションのアップグレード向けの deb パッケージをダウンロードします。
# ダウンロード後に SD カードに保存して、更新してください。
#
# システムのアップグレードには対応していません。
#
# pip install packaging requests
# python download_resolved_deps.py

#!/usr/bin/env python3
import os
import shutil
import re
import requests
from packaging.version import Version

BASE_URL = "https://repo.llm.m5stack.com/m5stack-apt-repo"
PACKAGES_URL = f"{BASE_URL}/dists/jammy/ax630c/binary-arm64/Packages"
OUTPUT_DIR = "llm_debs_resolved"
CONFIG_PATH = os.path.join(OUTPUT_DIR, "m5stack_update.config")

# 出力フォルダを削除して再作成
if os.path.exists(OUTPUT_DIR):
    print(f"⚠️  {OUTPUT_DIR} フォルダを削除して再作成します")
    shutil.rmtree(OUTPUT_DIR)
os.makedirs(OUTPUT_DIR)

# 初期化
with open(CONFIG_PATH, "w") as f:
    pass

r = requests.get(PACKAGES_URL)
r.raise_for_status()
entries = r.text.strip().split("\n\n")

package_info = {}
depends_map = {}

for entry in entries:
    name, version, filename, depends = None, None, None, []
    for line in entry.splitlines():
        if line.startswith("Package:"):
            name = line.split(": ", 1)[1]
        elif line.startswith("Version:"):
            version = line.split(": ", 1)[1]
        elif line.startswith("Filename:"):
            filename = line.split(": ", 1)[1]
        elif line.startswith("Depends:"):
            depends_str = line.split(": ", 1)[1]
            deps = [x.strip() for x in depends_str.split(",")]
            for dep in deps:
                m = re.match(r"(\S+)( \(>=? ([^)]+)\))?", dep)
                if m:
                    dep_name = m.group(1)
                    dep_ver = m.group(3)
                    depends.append((dep_name, dep_ver))
    if name and version and filename:
        package_info.setdefault(name, []).append((Version(version), filename))
    if name and depends:
        depends_map[name] = depends

selected = {}

for pkg, deps in depends_map.items():
    for dep_name, dep_version in deps:
        if dep_name not in package_info:
            continue
        candidates = package_info[dep_name]
        valid = [f for v, f in candidates if dep_version is None or v >= Version(dep_version)]
        if valid:
            selected[dep_name] = valid[-1]

for pkg, versions in package_info.items():
    latest = max(versions, key=lambda x: x[0])[1]
    selected.setdefault(pkg, latest)

with open(CONFIG_PATH, "a") as config:
    for pkg, file_path in selected.items():
        filename = file_path.split("/")[-1]
        url = f"{BASE_URL}/{file_path}"
        print(f"⬇️  Downloading {filename}")
        response = requests.get(url)
        response.raise_for_status()
        with open(os.path.join(OUTPUT_DIR, filename), "wb") as f:
            f.write(response.content)
        config.write(f"{filename}\n")

print(f"\n✅ ダウンロード完了！{len(selected)} 個の .deb を {OUTPUT_DIR}/ に保存しました。")
print(f"📄 m5stack_update.config にファイル名を記録しました。")
