/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_splitter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:16:29 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 16:40:15 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// A simplified function to split a string, handling basic quotes
char** shlex_split_c(const char* input_string, int* num_tokens) {
    char** tokens = NULL;
    int token_count = 0;
    int in_quote = 0; // 0: no quote, 1: single quote, 2: double quote
    int start_index = 0;
    int i;

    for (i = 0; input_string[i] != '\0'; i++) {
        if (input_string[i] == '\'' && in_quote == 0) {
            in_quote = 1;
            start_index = i; // Start token after the quote
        } else if (input_string[i] == '\"' && in_quote == 0) {
            in_quote = 2;
            start_index = i;
        } else if ((input_string[i] == '\'' && in_quote == 1) || (input_string[i] == '\"' && in_quote == 2)) {
            // End of a quoted section
            int token_len = i - start_index + 1;
            char* token = (char*)malloc(token_len + 1);
            ft_strlcpy(token, input_string + start_index, token_len + 1);
            token[token_len] = '\0';

            tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
            tokens[token_count++] = token;
            in_quote = 0;
            start_index = i + 1; // Start looking for next token after the closing quote
        } else if (ft_isspace(input_string[i]) && in_quote == 0) {
            // If not in a quote and whitespace is encountered, process the current token
            if (i > start_index) { // Only add if there's actual content
                int token_len = i - start_index;
                char* token = (char*)malloc(token_len + 1);
                ft_strlcpy(token, input_string + start_index, token_len + 1);
                token[token_len] = '\0';

                tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
                tokens[token_count++] = token;
            }
            start_index = i + 1; // Move to the character after the whitespace
        }
    }

    // Handle any remaining token after the loop
    if (i > start_index) {
        int token_len = i - start_index;
        char* token = (char*)malloc(token_len + 1);
        ft_strlcpy(token, input_string + start_index, token_len);
        token[token_len] = '\0';

        tokens = (char**)realloc(tokens, (token_count + 1) * sizeof(char*));
        tokens[token_count++] = token;
    }

    *num_tokens = token_count;
    return tokens;
}

// Function to free the allocated memory
void free_tokens(char** tokens, int num_tokens) {
    if (tokens) {
        for (int i = 0; i < num_tokens; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
}

int main() {
    const char* command_string = "this is 'my string' -A\"aaaa\" that --has=arguments";
    int count;
    char** args = shlex_split_c(command_string, &count);

    printf("Tokens:\n");
    for (int i = 0; i < count; i++) {
        printf("  %s\n", args[i]);
    }

    free_tokens(args, count);
    return 0;
}
