#include "document.h"
#include <stdio.h>

int init_document(Document *doc, const char *name) {
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }
    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int print_document(Document *doc) {
    int i, j, number_of_paragraphs;

    if (doc == NULL) {
        return FAILURE;
    }

    number_of_paragraphs = doc->number_of_paragraphs;

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", number_of_paragraphs);

    for (i = 0; i < number_of_paragraphs; i++) {
        int number_of_lines = doc->paragraphs[i].number_of_lines;

        if (number_of_lines > 0) {
            for (j = 0; j < number_of_lines; j++) {
                printf("%s\n", doc->paragraphs[i].lines[j]);
            }
            if (i < (number_of_paragraphs - 1)) {
                printf("\n");
            }
        }
    }

    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i, front = 0, end = 0, index = 0;

    if (doc != NULL) {
        index = doc->number_of_paragraphs;
        if (index < MAX_PARAGRAPHS && paragraph_number <= index) {

            if (paragraph_number == 0 && !index) {
                doc->paragraphs[0].number_of_lines = 0;
            }
            if (paragraph_number == doc->number_of_paragraphs) {
                end = doc->number_of_paragraphs;
                doc->paragraphs[end].number_of_lines = 0;
            } else {
                front = doc->number_of_paragraphs;
                for (i = front; i >= paragraph_number; i--) {
                    doc->paragraphs[i + 1] = doc->paragraphs[i];
                }
                doc->paragraphs[paragraph_number].number_of_lines = 0;
            }
            (doc->number_of_paragraphs)++;
            return SUCCESS;
        }
    }
    return FAILURE;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {
    int i, index;

    if (doc == NULL || paragraph_number < 1 || paragraph_number > doc->number_of_paragraphs || new_line == NULL || line_number < 0) {
        return FAILURE;
    }

    paragraph_number--;

    if (doc->paragraphs[paragraph_number].number_of_lines >= MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }

    index = doc->paragraphs[paragraph_number].number_of_lines;

    if (line_number >= 0 && line_number <= index) {
        for (i = index; i > line_number; i--) {
            strcpy(doc->paragraphs[paragraph_number].lines[i], doc->paragraphs[paragraph_number].lines[i - 1]);
        }

        strcpy(doc->paragraphs[paragraph_number].lines[line_number], new_line);
        doc->paragraphs[paragraph_number].number_of_lines++;

        return SUCCESS;
    }
    return FAILURE;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {
    if (doc == NULL || paragraph_number < 1 || paragraph_number > doc->number_of_paragraphs || number_of_lines == NULL) {
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    return SUCCESS;
}

int get_number_lines(Document *doc, int *number_of_lines) {
    int i, total_lines = 0;

    if (doc == NULL || number_of_lines == NULL) {
        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        total_lines += doc->paragraphs[i].number_of_lines;
    }

    *number_of_lines = total_lines;

    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    int i, index;

    if (doc == NULL || paragraph_number < 1 || paragraph_number > doc->number_of_paragraphs || new_line == NULL) {
        return FAILURE;
    }

    paragraph_number--;

    if (doc->paragraphs[paragraph_number].number_of_lines >= MAX_PARAGRAPH_LINES) {
        return FAILURE;
    }

    index = doc->paragraphs[paragraph_number].number_of_lines;

    strcpy(doc->paragraphs[paragraph_number].lines[index], new_line);
    doc->paragraphs[paragraph_number].number_of_lines++;

    return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i, number_of_lines;

    if (doc == NULL || paragraph_number < 1 || paragraph_number > doc->number_of_paragraphs || line_number < 1) {
        return FAILURE;
    }

    paragraph_number--;
    line_number--;

    number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;

    if (line_number >= number_of_lines) {
        return FAILURE;
    }

    for (i = line_number; i < number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph_number].lines[i], doc->paragraphs[paragraph_number].lines[i + 1]);
    }

    doc->paragraphs[paragraph_number].number_of_lines--;

    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
    int i = 0;

    if (doc == NULL && data == NULL && data_lines) {
        return FAILURE;
    }

    add_paragraph_after(doc, doc->number_of_paragraphs);

    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            add_paragraph_after(doc, doc->number_of_paragraphs);
        } else {
            append_line(doc, doc->number_of_paragraphs, data[i]);
        }
    }

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int paragraph_index, line_index, num_lines = 0;
    char *current, temp[MAX_STR_SIZE + 1] = {0};

    if (doc != NULL && target != NULL && replacement != NULL) {

        for (paragraph_index = 0; paragraph_index < doc->number_of_paragraphs; paragraph_index++) {
            num_lines = doc->paragraphs[paragraph_index].number_of_lines;

            for (line_index = 0; line_index < num_lines; line_index++) {

                current = strstr(doc->paragraphs[paragraph_index].lines[line_index], target);
                while (current != NULL) {

                    strcpy(temp, current);
                    strcpy(current, "");
                    strcat(current, replacement);
                    strcat(current + strlen(replacement), temp + strlen(target));

                    current = strstr(current + strlen(replacement), target);
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

int highlight_text(Document *doc, const char *target) {
    char highlighted[MAX_STR_SIZE + 1] = "";

    if (doc == NULL && target == NULL) {
        return FAILURE;
    }

    strcat(highlighted, HIGHLIGHT_START_STR);
    strcat(highlighted, target);
    strcat(highlighted, HIGHLIGHT_END_STR);

    replace_text(doc, target, highlighted);

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    replace_text(doc, target, "");

    return SUCCESS;
}
