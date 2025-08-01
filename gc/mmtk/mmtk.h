#ifndef MMTK_H
#define MMTK_H

/* Warning, this file is autogenerated by cbindgen from the mmtk-ruby repository. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct MMTk_Builder MMTk_Builder;
typedef struct MMTk_Mutator MMTk_Mutator;

typedef struct MMTk_ractor_cache *MMTk_VMThread;
typedef struct MMTk_ractor_cache *MMTk_VMMutatorThread;
typedef struct MMTk_GCThreadTLS *MMTk_VMWorkerThread;
typedef void *MMTk_Address;
typedef void *MMTk_ObjectReference;
typedef void *MMTk_NullableObjectReference;
typedef uint32_t MMTk_AllocationSemantics;


#define MMTk_OBJREF_OFFSET 8

#define MMTk_MIN_OBJ_ALIGN 8

#define MMTk_GC_THREAD_KIND_WORKER 1

typedef struct MMTk_RubyBindingOptions {
    bool ractor_check_mode;
    size_t suffix_size;
} MMTk_RubyBindingOptions;

typedef MMTk_ObjectReference (*MMTk_ObjectClosureFunction)(void*, void*, MMTk_ObjectReference, bool);

typedef struct MMTk_ObjectClosure {
    /**
     * The function to be called from C.
     */
    MMTk_ObjectClosureFunction c_function;
    /**
     * The pointer to the Rust-level closure object.
     */
    void *rust_closure;
} MMTk_ObjectClosure;

typedef struct MMTk_GCThreadTLS {
    int kind;
    void *gc_context;
    struct MMTk_ObjectClosure object_closure;
} MMTk_GCThreadTLS;

typedef struct MMTk_RubyUpcalls {
    void (*init_gc_worker_thread)(struct MMTk_GCThreadTLS *gc_worker_tls);
    bool (*is_mutator)(void);
    void (*stop_the_world)(void);
    void (*resume_mutators)(void);
    void (*block_for_gc)(MMTk_VMMutatorThread tls);
    size_t (*number_of_mutators)(void);
    void (*get_mutators)(void (*visit_mutator)(MMTk_Mutator*, void*), void *data);
    void (*scan_gc_roots)(void);
    void (*scan_objspace)(void);
    void (*scan_object_ruby_style)(MMTk_ObjectReference object);
    void (*call_gc_mark_children)(MMTk_ObjectReference object);
    void (*call_obj_free)(MMTk_ObjectReference object);
    size_t (*vm_live_bytes)(void);
    void (*update_global_tables)(int tbl_idx);
    int (*global_tables_count)(void);
    void (*update_finalizer_table)(void);
    bool (*special_const_p)(MMTk_ObjectReference object);
} MMTk_RubyUpcalls;

typedef struct MMTk_RawVecOfObjRef {
    MMTk_ObjectReference *ptr;
    size_t len;
    size_t capa;
} MMTk_RawVecOfObjRef;

bool mmtk_is_live_object(MMTk_ObjectReference object);

bool mmtk_is_reachable(MMTk_ObjectReference object);

MMTk_Builder *mmtk_builder_default(void);

void mmtk_init_binding(MMTk_Builder *builder,
                       const struct MMTk_RubyBindingOptions *_binding_options,
                       const struct MMTk_RubyUpcalls *upcalls,
                       MMTk_ObjectReference weak_reference_dead_value);

void mmtk_initialize_collection(MMTk_VMThread tls);

MMTk_Mutator *mmtk_bind_mutator(MMTk_VMMutatorThread tls);

void mmtk_destroy_mutator(MMTk_Mutator *mutator);

void mmtk_handle_user_collection_request(MMTk_VMMutatorThread tls, bool force, bool exhaustive);

void mmtk_set_gc_enabled(bool enable);

bool mmtk_gc_enabled_p(void);

MMTk_Address mmtk_alloc(MMTk_Mutator *mutator,
                        size_t size,
                        size_t align,
                        size_t offset,
                        MMTk_AllocationSemantics semantics);

void mmtk_post_alloc(MMTk_Mutator *mutator,
                     MMTk_ObjectReference refer,
                     size_t bytes,
                     MMTk_AllocationSemantics semantics);

void mmtk_add_obj_free_candidate(MMTk_ObjectReference object);

void mmtk_mark_weak(MMTk_ObjectReference *ptr);

void mmtk_remove_weak(const MMTk_ObjectReference *ptr);

void mmtk_object_reference_write_post(MMTk_Mutator *mutator, MMTk_ObjectReference object);

void mmtk_register_wb_unprotected_object(MMTk_ObjectReference object);

bool mmtk_object_wb_unprotected_p(MMTk_ObjectReference object);

void mmtk_enumerate_objects(void (*callback)(MMTk_ObjectReference, void*), void *data);

struct MMTk_RawVecOfObjRef mmtk_get_all_obj_free_candidates(void);

void mmtk_free_raw_vec_of_obj_ref(struct MMTk_RawVecOfObjRef raw_vec);

void mmtk_before_fork(void);

void mmtk_after_fork(MMTk_VMThread tls);

size_t mmtk_total_bytes(void);

size_t mmtk_used_bytes(void);

size_t mmtk_free_bytes(void);

MMTk_Address mmtk_starting_heap_address(void);

MMTk_Address mmtk_last_heap_address(void);

size_t mmtk_worker_count(void);

const uint8_t *mmtk_plan(void);

const uint8_t *mmtk_heap_mode(void);

size_t mmtk_heap_min(void);

size_t mmtk_heap_max(void);

bool mmtk_is_mmtk_object(MMTk_Address addr);

#endif  /* MMTK_H */
