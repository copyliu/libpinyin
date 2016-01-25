/* 
 *  libpinyin
 *  Library to deal with pinyin.
 *  
 *  Copyright (C) 2016 Peng Wu <alexepico@gmail.com>
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "phrase_large_table3.h"

namespace pinyin{


table_entry_header_t PhraseTableEntry::get_header() const {
    table_entry_header_t * head = (table_entry_header_t *) m_chunk.begin();
    return *head;
}

void PhraseTableEntry::set_header(table_entry_header_t header) {
    table_entry_header_t * head = (table_entry_header_t *) m_chunk.begin();
    *head = header;
}

/* search method */

int PhraseTableEntry::search(/* out */ PhraseTokens tokens) const {
    int result = SEARCH_NONE;

    const char * content = (char *) m_chunk.begin() +
        sizeof(table_entry_header_t);
    const phrase_token_t * begin = (phrase_token_t *) content;
    const phrase_token_t * end = (phrase_token_t *) m_chunk.end();

    const phrase_token_t * iter = NULL;
    GArray * array = NULL;

    for (iter = begin; iter != end; ++iter) {
        phrase_token_t token = *iter;

        /* filter out disabled sub phrase indices. */
        array = tokens[PHRASE_INDEX_LIBRARY_INDEX(token)];
        if (NULL == array)
            continue;

        result |= SEARCH_OK;

        g_array_append_val(array, token);
    }

    /* check SEARCH_CONTINUED flag in header */
    table_entry_header_t header = get_header();
    if (header & SEARCH_CONTINUED)
        result |= SEARCH_CONTINUED;

    return result;
}

/* add_index/remove_index method */

int PhraseTableEntry::add_index(/* in */ phrase_token_t token) {
    const char * content = (char *) m_chunk.begin() +
        sizeof(table_entry_header_t);
    const phrase_token_t * begin = (phrase_token_t *) content;
    const phrase_token_t * end = (phrase_token_t *) m_chunk.end();

    const phrase_token_t * cur_token;
    for (cur_token = begin; cur_token != end; ++cur_token) {
        if (*cur_token == token)
            return ERROR_INSERT_ITEM_EXISTS;
        if (*cur_token > token)
            break;
    }

    int offset = sizeof(table_entry_header_t) /* header */ +
        (cur_token - begin) * sizeof(phrase_token_t);
    m_chunk.insert_content(offset, &token, sizeof(phrase_token_t));
    return ERROR_OK;
}

int PhraseTableEntry::remove_index(/* in */ phrase_token_t token) {
    const char * content = (char *) m_chunk.begin() +
        sizeof(table_entry_header_t);
    const phrase_token_t * begin = (phrase_token_t *) content;
    const phrase_token_t * end = (phrase_token_t *) m_chunk.end();

    const phrase_token_t * cur_token;
    for (cur_token = begin; cur_token != end; ++cur_token) {
        if (*cur_token == token)
            break;
    }

    if (cur_token == end)
        return ERROR_REMOVE_ITEM_DONOT_EXISTS;

    int offset = sizeof(table_entry_header_t) /* header */ +
        (cur_token - begin) * sizeof(phrase_token_t);
    m_chunk.remove_content(offset, sizeof(phrase_token_t));
    return ERROR_OK;
}

/* get length method */

int PhraseTableEntry::get_length() const {
    const char * content = (char *) m_chunk.begin() +
        sizeof(table_entry_header_t);
    const phrase_token_t * begin = (phrase_token_t *) content;
    const phrase_token_t * end = (phrase_token_t *) m_chunk.end();

    return begin - end;
}

/* mask out method */

bool PhraseTableEntry::mask_out(phrase_token_t mask, phrase_token_t value) {
    const char * content = (char *) m_chunk.begin() +
        sizeof(table_entry_header_t);
    const phrase_token_t * begin = (phrase_token_t *) content;
    const phrase_token_t * end = (phrase_token_t *) m_chunk.end();

    const phrase_token_t * cur_token;
    for (cur_token = begin; cur_token != end; ++cur_token) {
        if ((*cur_token & mask) != value)
            continue;

        int offset = sizeof(table_entry_header_t) /* header */ +
            (cur_token - begin) * sizeof(phrase_token_t);
        m_chunk.remove_content(offset, sizeof(phrase_token_t));

        /* update chunk end. */
        end = (phrase_token_t *) m_chunk.end();
        --cur_token;
    }

    return true;
}


/* load text method */

bool PhraseLargeTable3::load_text(FILE * infile){
    char pinyin[256];
    char phrase[256];
    phrase_token_t token;
    size_t freq;

    while (!feof(infile)) {
        int num = fscanf(infile, "%256s %256s %u %ld",
                         pinyin, phrase, &token, &freq);

        if (4 != num)
            continue;

        if (feof(infile))
            break;

        glong phrase_len = g_utf8_strlen(phrase, -1);
        ucs4_t * new_phrase = g_utf8_to_ucs4(phrase, -1, NULL, NULL, NULL);
        add_index(phrase_len, new_phrase, token);

        g_free(new_phrase);
    }
    return true;
}


};
