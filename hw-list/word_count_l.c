/*
 * Implementation of the word_count interface using Pintos lists.
 *
 * You may modify this file, and are expected to modify it.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif
#include "word_count.h"

void init_words(word_count_list_t* wclist) {
   /* TODO */
  list_init(wclist);
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  int len = 0;
  struct list_elem *e;
  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
    len++;
  } 
  return len;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem *e;
  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
    word_count_t* w = list_entry(e, struct word_count, elem);
    if(strcmp(w->word, *word) == 0){
      return w;
    }
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  word_count_t *w = find_word(wclist, word);
  if( w != NULL){
    //we need to add new point in front
    w->count++;
  }else{
    if(w = malloc(sizeof(word_count_t)) != NULL){
      w->word = word;
      w->count = 1;
      
      struct list_elem *e = list_begin(wclist);
      list_insert(e, &w->elem);
    }
  }
  return w;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { /* TODO */
    struct list_elem *e;
    for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
      word_count_t *w = list_entry(e, struct word_count, elem);
      fprintf(outfile, "%s\t %d\n", w->word, w->count);
    }
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* TODO */
  bool (*less)(const word_count_t*, const word_count_t*) = aux;
  word_count_t *w1 = list_entry(ewc1, struct word_count, elem);
  word_count_t *w2 = list_entry(ewc2, struct word_count, elem);

  return less(w1, w2);
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  list_sort(wclist, less_list, less);
}
