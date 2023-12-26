#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// 打印压缩JSON字符串到文件
void print_json_file_compact(char* json, FILE* output_file) {
    while (*json) {
        if (!is_whitespace(*json)) { // 非空格字符
            fputc(*json, output_file);
        }
        json++;
    }
}