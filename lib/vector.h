#define PLEB_VECTOR_INIT_SIZE 4

// #ifndef PTL_NODE
//     #define PTL_NODE
//     typedef struct PtlNode{
//         void* p_val;
//         struct PtlNode* p_prev;
//         struct PtlNode* p_next;
//     }PtlNode;
// #endif

#define ptl_vector_add(p_v, val) \
    do { \
        char *c=malloc(sizeof(val)); \
        memcpy(c, &val, sizeof(val)); \
        __ptl_vector_add(p_v, c); \
    } while(0);

#define ptl_vector_set(p_v, idx, val) \
    do { \
        char *c=malloc(sizeof(val)); \
        memcpy(c, &val, sizeof(val)); \
        __ptl_vector_set(p_v, idx, c); \
    } while(0);

#define ptl_vector_get(type_of, p_v, idx)\
    *(type_of*)__ptl_vector_get(p_v, idx) // this method is unsafe when accessing data outside bounds
    // (__ptl_vector_get(p_v, idx)!=NULL) ?*(type_of*)__ptl_vector_get(p_v, idx) :*(type_of*)malloc(sizeof(type_of)) // This method results in memory leaks 

#define ptl_vector_init(type_of, p_v)\
    __ptl_vector_init(p_v, sizeof(type_of))

typedef struct PtlVector{
    void **items;
    // PtlNode *p_tail, *p_head;
    int size;
    int capacity;
    int len;
    // another int value can fit in here
}PtlVector;

void __ptl_vector_init(PtlVector*, int);
int ptl_vector_len(PtlVector*);
static void __ptl_vector_resize(PtlVector*, int);
void __ptl_vector_add(PtlVector*, void*);
void __ptl_vector_set(PtlVector*, int, void*);
void* __ptl_vector_get(PtlVector*, int);
void ptl_vector_delete(PtlVector*, int);
void ptl_vector_free(PtlVector*);

//Implementation

void __ptl_vector_init(PtlVector *p_v, int size){
    // ptl_vector_free(p_v);
    p_v->len = 0;
    p_v->size = size;
    p_v->capacity = PLEB_VECTOR_INIT_SIZE;
    p_v->items = malloc(p_v->capacity * sizeof(void*));
    // p_v->p_head = malloc(sizeof(PtlNode));
    // p_v->p_tail = p_v->p_head;
}

int ptl_vector_len(PtlVector *p_v){
    return p_v->len;
}

static void __ptl_vector_resize(PtlVector *p_v, int new_capacity){
    void **new_items = realloc(p_v->items, new_capacity*sizeof(void*));
    if(new_items){
        p_v->items = new_items;
        p_v->capacity = new_capacity;
    }
}

void __ptl_vector_add(PtlVector *p_v, void *item){
    if(p_v->capacity == p_v->len){
        __ptl_vector_resize(p_v, (p_v->capacity)*2);
    }
    
    // PtlNode* p_node = malloc(sizeof(PtlNode));
    
    // p_node->p_val = item;
    // p_node->p_prev = p_v->p_head;
    // p_node->p_next = NULL;
    
    // p_v->p_head->p_next = p_node;
    // p_v->p_head = p_node;
    
    p_v->items[p_v->len] = item;
    // p_v->items[p_v->len] = malloc(p_v->size);
    // memcpy(p_v->items[p_v->len], item, p_v->size);
    p_v->len++;
}

void __ptl_vector_set(PtlVector* p_v, int idx, void* item){
    if(idx >= 0 && idx < p_v->len){
        p_v->items[idx] = item;
    }
}

void* __ptl_vector_get(PtlVector* p_v, int idx){
    if(idx >= 0 && idx < p_v->len){
        return p_v->items[idx];
    }
    return NULL;
}

void ptl_vector_delete(PtlVector* p_v, int idx){
    if(idx < 0 || idx >= p_v->len){
        return;
    }

    // free(p_v->items[idx]);
    p_v->items[idx] = NULL;  
    
    for(int i=idx; i<p_v->len-1; i++){
        p_v->items[i] = p_v->items[i+1];
    }
    // p_v->items[p_v->len-1]=NULL;

    p_v->len--;

    if(p_v->len>0 && p_v->len == p_v->capacity/4){
        __ptl_vector_resize(p_v, p_v->capacity/2);
    }
}

void ptl_vector_free(PtlVector* p_v){
    for(int i=0; i<p_v->len; i++){
        free(p_v->items[i]);
    }
    free(p_v->items);
}