#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable : 4996)

#define INDENT_SPACES 4 // 缩进空格数

// 函数声明
// 打印JSON
void print_json(char *json, FILE *output_file);
// 验证JSON
int is_json(const char *filename);
// 去除空格
void remove_spaces(char *str);
// 输出JSON文件(压缩)
void print_json_file_compact(char *json, FILE *output_file);
// 判断是否是空格
int is_whitespace(char c);

int main() {

    // 输入JSON文件的路径文件名
    char filename[100];
    printf("请输入文件名称: ");
    scanf("%s", filename);

    // 打开并读取文件内容
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("文件打开错误.\n");
        system("pause"); // 按任意键继续
        return 1;
    }

    // 获取文件大小
    int file_size = 0;
    while (fgetc(file) != EOF) {
        file_size++;
    }
    fseek(file, 0, SEEK_SET); // 将文件指针重新指向文件开头

    // 读取文件内容
    char *json_content = (char *) malloc(file_size + 1);
    fread(json_content, 1, file_size, file);
    json_content[file_size] = '\0';

    // 关闭文件
    fclose(file);

    // 检查文件内容是否符合JSON格式

    if (!is_json(filename)) {
        free(json_content);
        system("pause"); // 按任意键继续
        return 1;
    }
    remove_spaces(json_content);

    // 按照文件格式将JSON字符串显示在屏幕上
    printf("格式化输出为:\n");
    print_json(json_content, stdout); // stdout 是标准输出流
    printf("\n");

    // 按照文件格式将JSON字符串保存在指定的文本文件中
    char output_filename[100];
    printf("请输出想要格式化保存json的文件名: ");
    scanf("%s", output_filename);
    FILE *output_file = fopen(output_filename, "w");
    print_json(json_content, output_file);
    fclose(output_file);
    printf("格式化保存成功，文件名为 %s.\n", output_filename);

    printf("请输出想要压缩保存json的文件名: ");
    scanf("%s", output_filename);
    FILE *output_file2 = fopen(output_filename, "w");
    print_json_file_compact(json_content, output_file2);
    fclose(output_file2);
    printf("压缩保存成功，文件名为 %s.\n", output_filename);

    // 释放内存
    free(json_content);
    system("pause"); // 按任意键继续
    return 0;
}

// 删除字符串中的空格
void remove_spaces(char *str) {
    if (str == NULL) {
        return;
    }
    // 定义两个指针，一个用于遍历原始字符串，另一个用于保存非空格字符
    char *source = str;
    char *destination = str;
    int inside = 1; // 1表示不在双引号内，-1表示在双引号内

    // 遍历字符串
    while (*source) {
        if (*source == '\"') {
            inside *= -1;// 改变inside的值
        }
        if ((*source != ' '&& *source!= '\n' && *source!= '\r' && *source!= '\t') || inside == -1) {
            *destination = *source; // 保存非空格字符
            destination++;
        }

        source++;
    }
    // 在新字符串的末尾添加终止符
    *destination = '\0';
}

// 打印JSON字符串到文件，使用循环
void print_json(char *json, FILE *output_file) {
    int indent = 0;
    int flag = 0;
    while (*json) { // 遍历字符串
        if (*json == '{' || *json == '[') {
//            for (int i = 0; i < indent - INDENT_SPACES; i++) { // 打印缩进空格
//                fputc(' ', output_file);
//            }
            fputc(*json, output_file);
            indent += INDENT_SPACES; // 增加缩进
            if (*(json + 1) != '\n'){
                fputc('\n', output_file);
                for (int i = 0; i < indent; i++) { // 打印缩进空格
                    fputc(' ', output_file);
                }
            } else
                for (int i = 0; i < indent; i++) { // 打印缩进空格
                fputc(' ', output_file);
            }
        } else if (*json == '}' || *json == ']') {
            if (*(json - 1) != '\n'){
                fputc('\n', output_file);
            }
            for (int i = 0; i < indent - INDENT_SPACES; i++) {
                fputc(' ', output_file);
            }
            fputc(*json, output_file);
            indent -= INDENT_SPACES; // 减少缩进
        } else if (*json == ':') {
            fputc(*json, output_file);
            fputc(' ', output_file);
        } else if (*json == ',') {
            fputc(*json, output_file);
            fputc(' ', output_file);
            if (*(json + 1) != '\n'){
                fputc('\n', output_file);
                for (int i = 0; i < indent; i++) { // 打印缩进空格
                    fputc(' ', output_file);
                }
            }
        } else if (*json == '\n') {
            flag = 1;
            fputc('\n', output_file);
        } else {
            if (flag) {
                flag = 0;
                for (int i = 0; i < indent; i++) { // 打印缩进空格
                    fputc(' ', output_file);
                }
            }
            fputc(*json, output_file);
        }

        json++;
    }
}

// 判断字符是否为空格
int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// 打印压缩JSON字符串到文件
void print_json_file_compact(char *json, FILE *output_file) {
    while (*json) {
        if (!is_whitespace(*json)) { // 非空格字符
            fputc(*json, output_file);
        }
        json++;
    }
}

// 检查JSON格式是否有效，并记录错误位置
int is_json(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    int line = 1; // 当前行数
    int column = 0; // 当前列数
    char ch;
    int stack1 = 0; // 花括号 { 的匹配栈
    int stack2 = 0; // 方括号 [ 的匹配栈
    int inString = 0; // 是否在字符串中
    int lastChar = 0; // 上一个字符

    while ((ch = (char)fgetc(file)) != EOF) {
        column++;
        if (ch == '\n') {
            line++;
            column = 0;
        } else if (ch == '{') {
            if (!inString) {
                stack1++;
            }
        } else if (ch == '}') {
            if (!inString) {
                stack1--;
                if (stack1 < 0) {
                    printf("错误：缺少左花括号 { 在第 %d 行，第 %d 列\n", line, column);
                    fclose(file);
                    return 0;
                }
            }
        } else if (ch == '[') {
            if (!inString) {
                stack2++;
            }
        } else if (ch == ']') {
            if (!inString) {
                stack2--;
                if (stack2 < 0) {
                    printf("错误：缺少左方括号 [ 在第 %d 行，第 %d 列\n", line, column);
                    fclose(file);
                    return 0;
                }
            }
        } else if (ch == '"') {
            if (inString) {
                inString = 0;
            } else {
                inString = 1;
            }
        } else if (ch == ':' && !inString) {
            if (lastChar != '"') {
                printf("错误：键必须是字符串，在第 %d 行，第 %d 列\n", line, column);
                fclose(file);
                return 0;
            }
        } else if (ch == ',' && !inString) {
            if (lastChar == '{' || lastChar == '[' || lastChar == ',') {
                printf("错误：逗号后面缺少元素，在第 %d 行，第 %d 列\n", line, column);
                fclose(file);
                return 0;
            }
        }
        lastChar = (int)ch;
    }
    fclose(file);
    if (stack1 != 0) {
        printf("错误：缺少右花括号 }\n");
        return 0;
    }
    if (stack2 != 0) {
        printf("错误：缺少右方括号 ]\n");
        return 0;
    }
    return 1;
}

