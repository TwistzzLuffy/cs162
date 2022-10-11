/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
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
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t* wclist) { /* TODO */
  init_words(wclist);
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&wclist->lock, &attr);
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  pthread_mutex_lock(&wclist->lock);
  size_t len = 0;// i think there are needed a lock 
  struct list_elem *e;
  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
    len += 1;
  } 
  pthread_mutex_unlock(&wclist->lock);
  return len;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem *e;
  //there are needed a lock to protect the next(e);
  pthread_mutex_lock(&wclist->lock);
  for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
    word_count_t *w = list_entry(e, struct word_count, elem);
    if(strcmp(word, w->word) == 0){
      pthread_mutex_unlock(&wclist->lock);
      return w;
    }
  }
  pthread_mutex_unlock(&wclist->lock);
  return NULL;
}


word_count_t* add_word(word_count_list_t* wclist, char* word) {
  pthread_mutex_lock(&wclist->lock);
  word_count_t* word_entry = find_word(wclist, word);
  if (word_entry) {
    /* If present. */
    word_entry->count++;
  } else {
    /* Create and insert new word. */
    word_count_t* new_word_entry = (word_count_t*)malloc(sizeof(word_count_t));
    new_word_entry->count = 1;
    new_word_entry->word = word;
    list_push_back(wclist, &new_word_entry->elem);
    word_entry = new_word_entry;
  }
  pthread_mutex_unlock(&wclist->lock);
  return word_entry;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { /* TODO */
    struct list_elem *e;
    for(e = list_begin(wclist); e != list_end(wclist); e = list_next(e)){
      struct word_count *w = list_entry(e, struct word_count, elem);
      fprintf(outfile, "%8d\t%s\n", w->count, w->word);
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
  /* TODO */
  list_sort(wclist, less_list, less);
}
