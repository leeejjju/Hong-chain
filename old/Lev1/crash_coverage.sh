#!/bin/bash

mkdir -p gcov/crash

# 3. make crash_btree
# crash file의 커버리지를 재기 위한 trace-pc-fo를 붙인 btree 실행 파일 생성
make crash_btree

# 4. outputs/defaults/crashes/의 모든 파일을 하나씩 ./crash_btree.out 에 돌리고
# 결과를 gcov/crash/에 파일 이름이 basename "$file"로 저장되도록 해줘
for file in outputs/default/crashes/*; do
    filename=$(basename "$file")
    if [[ $filename == id* ]]; then
        # crash_btree를 실행하고 결과를 coverage_log에 저장합니다.
        ./crash_btree.out < "$file" 

        # 결과를 원하는 위치에 저장합니다.
        mv coverage_log "gcov/crash/${filename}.coverage_log"
    fi
done

