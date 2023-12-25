#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma warning(disable : 4996)

#define INDENT_SPACES 4 // �����ո���

// ��������
// ��ӡJSON
void print_json(char* json, FILE* output_file);
// ��֤JSON
int is_json(const char* filename);
// ȥ���ո�
void remove_spaces(char* str);
// ���JSON�ļ�(ѹ��)
void print_json_file_compact(char* json, FILE* output_file);
// �ж��Ƿ��ǿո�
int is_whitespace(char c);

int main() {

    // ����JSON�ļ���·���ļ���
    char filename[100];
    printf("�������ļ�����: ");
    scanf("%s", filename);

    // �򿪲���ȡ�ļ�����
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�ļ��򿪴���.\n");
        system("pause"); // �����������
        return 1;
    }

    // ��ȡ�ļ���С
    int file_size = 0;
    while (fgetc(file) != EOF) {
        file_size++;
    }
    fseek(file, 0, SEEK_SET); // ���ļ�ָ������ָ���ļ���ͷ

    // ��ȡ�ļ�����
    char* json_content = (char*)malloc(file_size + 1);
    fread(json_content, 1, file_size, file);
    json_content[file_size] = '\0';

    // �ر��ļ�
    fclose(file);

    // ����ļ������Ƿ����JSON��ʽ

    if (!is_json(filename)) {
        free(json_content);
        system("pause"); // �����������
        return 1;
    }
    remove_spaces(json_content);

    // �����ļ���ʽ��JSON�ַ�����ʾ����Ļ��
    printf("��ʽ�����Ϊ:\n");
    print_json(json_content, stdout); // stdout �Ǳ�׼�����
    printf("\n");

    // �����ļ���ʽ��JSON�ַ���������ָ�����ı��ļ���
    char output_filename[100];
    printf("�������Ҫ��ʽ������json���ļ���: ");
    scanf("%s", output_filename);
    FILE* output_file = fopen(output_filename, "w");
    print_json(json_content, output_file);
    fclose(output_file);
    printf("��ʽ������ɹ����ļ���Ϊ %s.\n", output_filename);

    printf("�������Ҫѹ������json���ļ���: ");
    scanf("%s", output_filename);
    FILE* output_file2 = fopen(output_filename, "w");
    print_json_file_compact(json_content, output_file2);
    fclose(output_file2);
    printf("ѹ������ɹ����ļ���Ϊ %s.\n", output_filename);

    // �ͷ��ڴ�
    free(json_content);
    system("pause"); // �����������
    return 0;
}

// ɾ���ַ����еĿո�
void remove_spaces(char* str) {
    if (str == NULL) {
        return;
    }
    // ��������ָ�룬һ�����ڱ���ԭʼ�ַ�������һ�����ڱ���ǿո��ַ�
    char* source = str;
    char* destination = str;
    int inside = 1; // 1��ʾ����˫�����ڣ�-1��ʾ��˫������

    // �����ַ���
    while (*source) {
        if (*source == '\"') {
            inside *= -1;// �ı�inside��ֵ
        }
        if ((*source != ' ' && *source != '\n' && *source != '\r' && *source != '\t') || inside == -1) {
            *destination = *source; // ����ǿո��ַ�
            destination++;
        }

        source++;
    }
    // �����ַ�����ĩβ�����ֹ��
    *destination = '\0';
}

// ��ӡJSON�ַ������ļ���ʹ��ѭ��
void print_json(char* json, FILE* output_file) {
    int indent = 0;
    int flag = 0;
    while (*json) { // �����ַ���
        if (*json == '{' || *json == '[') {
            //            for (int i = 0; i < indent - INDENT_SPACES; i++) { // ��ӡ�����ո�
            //                fputc(' ', output_file);
            //            }
            fputc(*json, output_file);
            indent += INDENT_SPACES; // ��������
            if (*(json + 1) != '\n') {
                fputc('\n', output_file);
                for (int i = 0; i < indent; i++) { // ��ӡ�����ո�
                    fputc(' ', output_file);
                }
            }
            else
                for (int i = 0; i < indent; i++) { // ��ӡ�����ո�
                    fputc(' ', output_file);
                }
        }
        else if (*json == '}' || *json == ']') {
            if (*(json - 1) != '\n') {
                fputc('\n', output_file);
            }
            for (int i = 0; i < indent - INDENT_SPACES; i++) {
                fputc(' ', output_file);
            }
            fputc(*json, output_file);
            indent -= INDENT_SPACES; // ��������
        }
        else if (*json == ':') {
            fputc(*json, output_file);
            fputc(' ', output_file);
        }
        else if (*json == ',') {
            fputc(*json, output_file);
            fputc(' ', output_file);
            if (*(json + 1) != '\n') {
                fputc('\n', output_file);
                for (int i = 0; i < indent; i++) { // ��ӡ�����ո�
                    fputc(' ', output_file);
                }
            }
        }
        else if (*json == '\n') {
            flag = 1;
            fputc('\n', output_file);
        }
        else {
            if (flag) {
                flag = 0;
                for (int i = 0; i < indent; i++) { // ��ӡ�����ո�
                    fputc(' ', output_file);
                }
            }
            fputc(*json, output_file);
        }

        json++;
    }
}

// �ж��ַ��Ƿ�Ϊ�ո�
int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// ��ӡѹ��JSON�ַ������ļ�
void print_json_file_compact(char* json, FILE* output_file) {
    while (*json) {
        if (!is_whitespace(*json)) { // �ǿո��ַ�
            fputc(*json, output_file);
        }
        json++;
    }
}

// ���JSON��ʽ�Ƿ���Ч������¼����λ��
int is_json(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�޷����ļ� %s\n", filename);
        return 0;
    }
    int line = 1; // ��ǰ����
    int column = 0; // ��ǰ����
    char ch;
    int stack1 = 0; // ������ { ��ƥ��ջ
    int stack2 = 0; // ������ [ ��ƥ��ջ
    int inString = 0; // �Ƿ����ַ�����
    int lastChar = 0; // ��һ���ַ�

    while ((ch = (char)fgetc(file)) != EOF) {
        column++;
        if (ch == '\n') {
            line++;
            column = 0;
        }
        else if (ch == '{') {
            if (!inString) {
                stack1++;
            }
        }
        else if (ch == '}') {
            if (!inString) {
                stack1--;
                if (stack1 < 0) {
                    printf("����ȱ�������� { �ڵ� %d �У��� %d ��\n", line, column);
                    fclose(file);
                    return 0;
                }
            }
        }
        else if (ch == '[') {
            if (!inString) {
                stack2++;
            }
        }
        else if (ch == ']') {
            if (!inString) {
                stack2--;
                if (stack2 < 0) {
                    printf("����ȱ�������� [ �ڵ� %d �У��� %d ��\n", line, column);
                    fclose(file);
                    return 0;
                }
            }
        }
        else if (ch == '"') {
            if (inString) {
                inString = 0;
            }
            else {
                inString = 1;
            }
        }
        else if (ch == ':' && !inString) {
            if (lastChar != '"') {
                printf("���󣺼��������ַ������ڵ� %d �У��� %d ��\n", line, column);
                fclose(file);
                return 0;
            }
        }
        else if (ch == ',' && !inString) {
            if (lastChar == '{' || lastChar == '[' || lastChar == ',') {
                printf("���󣺶��ź���ȱ��Ԫ�أ��ڵ� %d �У��� %d ��\n", line, column);
                fclose(file);
                return 0;
            }
        }
        lastChar = (int)ch;
    }
    fclose(file);
    if (stack1 != 0) {
        printf("����ȱ���һ����� }\n");
        return 0;
    }
    if (stack2 != 0) {
        printf("����ȱ���ҷ����� ]\n");
        return 0;
    }
    return 1;
}

