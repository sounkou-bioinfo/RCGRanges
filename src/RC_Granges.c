#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "cgranges.h"

// forwar declare all sexp functions



///typedef struct {
//	int64_t n_r, m_r;     // number and max number of intervals
//	cr_intv_t *r;         // list of intervals (of size _n_r_)
//	int32_t n_ctg, m_ctg; // number and max number of contigs
//	cr_ctg_t *ctg;        // list of contigs (of size _n_ctg_)
//	void *hc;             // dictionary for converting contig names to integers
//} cgranges_t;





// Finalizer to free cgranges_t object
void RC_cr_destroy(SEXP cr_extptr) {
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr != NULL) {
        cr_destroy(cr_ptr);
        R_ClearExternalPtr(cr_extptr);
    }
}

// intialize a CGRanges object and return as an external pointer


SEXP RC_cr_init() {
    cgranges_t *cr_ptr = cr_init();
    if (cr_ptr == NULL) {
        Rf_error("Failed to initialize cgranges_t object.");
    }
    SEXP cr_extptr = R_MakeExternalPtr(cr_ptr, R_NilValue, R_NilValue);
    R_RegisterCFinalizerEx(cr_extptr, (R_CFinalizer_t)RC_cr_destroy, TRUE);
    return cr_extptr;

}
// add intervals to a CGRanges object
SEXP RC_cr_add(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en, SEXP label_int) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    // check if inputs ctg, st, en, label_int are of correct types
    // numeric and same length
    if(TYPEOF(ctg) != STRSXP) {
        Rf_error("ctg must be of character string type");
    }
    if(TYPEOF(st) != INTSXP ) {
        Rf_error("st must be of integer type");
    }
    if(TYPEOF(en) != INTSXP ) {
        Rf_error("en must be of integer type");
    }
    if(TYPEOF(label_int) != INTSXP ) {
        Rf_error("label_int must be of integer type");
    }
    R_xlen_t n = XLENGTH(ctg);
    if(XLENGTH(st) != n || XLENGTH(en) != n || XLENGTH(label_int) != n) {
        Rf_error("ctg, st, en, and label_int must have the same length.");
    }
    // add intervals
    for(R_xlen_t i = 0; i < n; i++) {
        const char *ctg_str = CHAR(STRING_ELT(ctg, i));
        int32_t st_val = INTEGER(st)[i];
        int32_t en_val = INTEGER(en)[i];
        if(st_val > en_val) {
            Rf_error("Start position cannot be greater than end position.");
        }
        int32_t label_val = INTEGER(label_int)[i];
        cr_add(cr_ptr, ctg_str, st_val, en_val, label_val);
    }
    return R_NilValue;
}

// index intervals in a CGRanges object
SEXP RC_cr_index(SEXP cr_extptr) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    // index intervals
    cr_index(cr_ptr);
    return R_NilValue;
}

SEXP RC_cr_add_ctg(SEXP cr_extptr, SEXP ctg, SEXP len) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    if(TYPEOF(ctg) != STRSXP || XLENGTH(ctg) != 1) {
        Rf_error("ctg must be a single character string.");
    }
    if(TYPEOF(len) != INTSXP || XLENGTH(len) != 1) {
        Rf_error("len must be a single integer.");
    }
    const char *ctg_str = CHAR(STRING_ELT(ctg, 0));
    int32_t len_val = INTEGER(len)[0];
    int32_t ctg_id = cr_add_ctg(cr_ptr, ctg_str, len_val);
    return Rf_ScalarInteger(ctg_id);
}

SEXP RC_cr_get_ctg(SEXP cr_extptr, SEXP ctg) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    if(TYPEOF(ctg) != STRSXP || XLENGTH(ctg) != 1) {
        Rf_error("ctg must be a single character string.");
    }
    const char *ctg_str = CHAR(STRING_ELT(ctg, 0));
    int32_t ctg_id = cr_get_ctg(cr_ptr, ctg_str);
    return Rf_ScalarInteger(ctg_id);
}

SEXP RC_cr_contain(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    if(TYPEOF(ctg) != STRSXP || XLENGTH(ctg) != 1) {
        Rf_error("ctg must be a single character string.");
    }
    if(TYPEOF(st) != INTSXP || XLENGTH(st) != 1) {
        Rf_error("st must be a single integer.");
    }
    if(TYPEOF(en) != INTSXP || XLENGTH(en) != 1) {
        Rf_error("en must be a single integer.");
    }
    const char *ctg_str = CHAR(STRING_ELT(ctg, 0));
    int32_t st_val = INTEGER(st)[0];
    int32_t en_val = INTEGER(en)[0];
    int64_t *b = NULL;
    int64_t m_b = 0;
    int64_t n_contain = cr_contain(cr_ptr, ctg_str, st_val, en_val, &b, &m_b);
    SEXP result = PROTECT(Rf_allocVector(INTSXP, n_contain));
    for(int64_t i = 0; i < n_contain; i++) {
        INTEGER(result)[i] = (int32_t)b[i];
    }
    free(b);
    UNPROTECT(1);
    return result;
}
SEXP RC_cr_contain_vectorized(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    R_xlen_t n = XLENGTH(ctg);
    if(XLENGTH(st) != n || XLENGTH(en) != n) {
        Rf_error("ctg, st, and en must have the same length.");
    }
    SEXP result_list = PROTECT(Rf_allocVector(VECSXP, n));
    for(R_xlen_t i = 0; i < n; i++) {
        const char *ctg_str = CHAR(STRING_ELT(ctg, i));
        int32_t st_val = INTEGER(st)[i];
        int32_t en_val = INTEGER(en)[i];
        int64_t *b = NULL;
        int64_t m_b = 0;
        int64_t n_contain = cr_contain(cr_ptr, ctg_str, st_val, en_val, &b, &m_b);
        SEXP result = PROTECT(Rf_allocVector(INTSXP, n_contain));
        for(int64_t j = 0; j < n_contain; j++) {
            INTEGER(result)[j] = (int32_t)b[j];
        }
        free(b);
        SET_VECTOR_ELT(result_list, i, result);
        UNPROTECT(1);
    }
    UNPROTECT(1);
    return result_list;
}
// overalap
SEXP RC_cr_overlap(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    if(TYPEOF(ctg) != STRSXP || XLENGTH(ctg) != 1) {
        Rf_error("ctg must be a single character string.");
    }
    if(TYPEOF(st) != INTSXP || XLENGTH(st) != 1) {
        Rf_error("st must be a single integer.");
    }
    if(TYPEOF(en) != INTSXP || XLENGTH(en) != 1) {
        Rf_error("en must be a single integer.");
    }
    const char *ctg_str = CHAR(STRING_ELT(ctg, 0));
    int32_t st_val = INTEGER(st)[0];
    int32_t en_val = INTEGER(en)[0];
    int64_t *b = NULL;
    int64_t m_b = 0;
    int64_t n_overlap = cr_overlap(cr_ptr, ctg_str, st_val, en_val, &b, &m_b);
    SEXP result = PROTECT(Rf_allocVector(INTSXP, n_overlap));
    for(int64_t i = 0; i < n_overlap; i++) {
        // Return 1-based indices to R
        INTEGER(result)[i] = (int32_t)b[i] + 1;
    }
    free(b);
    UNPROTECT(1);
    return result;
}
SEXP RC_cr_overlap_vectorized(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    R_xlen_t n = XLENGTH(ctg);
    if(XLENGTH(st) != n || XLENGTH(en) != n) {
        Rf_error("ctg, st, and en must have the same length.");
    }
    SEXP result_list = PROTECT(Rf_allocVector(VECSXP, n));
    for(R_xlen_t i = 0; i < n; i++) {
        const char *ctg_str = CHAR(STRING_ELT(ctg, i));
        int32_t st_val = INTEGER(st)[i];
        int32_t en_val = INTEGER(en)[i];
        int64_t *b = NULL;
        int64_t m_b = 0;
        int64_t n_overlap = cr_overlap(cr_ptr, ctg_str, st_val, en_val, &b, &m_b);
        SEXP result = PROTECT(Rf_allocVector(INTSXP, n_overlap));
        for(int64_t j = 0; j < n_overlap; j++) {
            // Return 1-based indices to R
            INTEGER(result)[j] = (int32_t)b[j] + 1;
        }
        free(b);
        SET_VECTOR_ELT(result_list, i, result);
        UNPROTECT(1);
    }
    UNPROTECT(1);
    return result_list;
}

// get chrom, start, end, label, 


SEXP RC_get_cr_intervals(SEXP cr_extptr, SEXP indexed) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    int is_indexed = asLogical(indexed);
    int64_t n_r = cr_ptr->n_r;
    SEXP result_list = PROTECT(Rf_allocVector(VECSXP, 5));
    SEXP chrom_vec = PROTECT(Rf_allocVector(STRSXP, n_r));
    SEXP start_vec = PROTECT(Rf_allocVector(INTSXP, n_r));
    SEXP end_vec = PROTECT(Rf_allocVector(INTSXP, n_r));
    SEXP label_vec = PROTECT(Rf_allocVector(INTSXP, n_r));
    for(int64_t i = 0; i < n_r; i++) {
        int32_t ctg_id;
        if (is_indexed) {
            ctg_id = -1;
            for (int32_t c = 0; c < cr_ptr->n_ctg; ++c) {
                if (i >= cr_ptr->ctg[c].off && i < cr_ptr->ctg[c].off + cr_ptr->ctg[c].n) {
                    ctg_id = c;
                    break;
                }
            }
            if (ctg_id < 0) {
                Rf_error("Could not determine contig ID for interval %lld", (long long)i);
            }
        } else {
            ctg_id = (int32_t)(cr_ptr->r[i].x >> 32);
            if (ctg_id < 0 || ctg_id >= cr_ptr->n_ctg) {
                Rf_error("Invalid contig ID in interval data (index %lld, ctg_id %d, n_ctg %d).", (long long)i, ctg_id, cr_ptr->n_ctg);
            }
        }
        const char *ctg_name = cr_ptr->ctg[ctg_id].name;
        SET_STRING_ELT(chrom_vec, i, Rf_mkChar(ctg_name));
        INTEGER(start_vec)[i] = cr_start(cr_ptr, i);
        INTEGER(end_vec)[i] = cr_end(cr_ptr, i);
        INTEGER(label_vec)[i] = cr_label(cr_ptr, i);
    }
    SET_VECTOR_ELT(result_list, 0, chrom_vec);
    SET_VECTOR_ELT(result_list, 1, start_vec);
    SET_VECTOR_ELT(result_list, 2, end_vec);
    SET_VECTOR_ELT(result_list, 3, label_vec);
    UNPROTECT(5);
    return result_list;
}

SEXP RC_get_cr_contigs(SEXP cr_extptr) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    int32_t n_ctg = cr_ptr->n_ctg;
    SEXP result_list = PROTECT(Rf_allocVector(VECSXP, 2));
    SEXP name_vec = PROTECT(Rf_allocVector(STRSXP, n_ctg));
    SEXP len_vec = PROTECT(Rf_allocVector(INTSXP, n_ctg));
    for(int32_t i = 0; i < n_ctg; i++) {
        const char *ctg_name = cr_ptr->ctg[i].name;
        SET_STRING_ELT(name_vec, i, Rf_mkChar(ctg_name));
        INTEGER(len_vec)[i] = cr_ptr->ctg[i].len;
    }
    SET_VECTOR_ELT(result_list, 0, name_vec);
    SET_VECTOR_ELT(result_list, 1, len_vec);
    UNPROTECT(3);
    return result_list;
}

SEXP RC_get_cr_n_intervals(SEXP cr_extptr) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    int64_t n_r = cr_ptr->n_r;
    return Rf_ScalarReal((double)n_r);
}

SEXP RC_get_cr_n_contigs(SEXP cr_extptr) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    int32_t n_ctg = cr_ptr->n_ctg;
    return Rf_ScalarInteger(n_ctg);
}
SEXP RC_cr_from_overlap(SEXP cr_extptr, SEXP ctg, SEXP st, SEXP en) {
    // check inputs
    if(TYPEOF(cr_extptr) != EXTPTRSXP) {
        Rf_error("First argument must be an external pointer to a cgranges_t object.");
    }
    cgranges_t *cr_ptr = (cgranges_t *)R_ExternalPtrAddr(cr_extptr);
    if (cr_ptr == NULL) {
        Rf_error("cgranges_t object has been freed.");
    }
    if(TYPEOF(ctg) != STRSXP || XLENGTH(ctg) != 1) {
        Rf_error("ctg must be a single character string.");
    }
    if(TYPEOF(st) != INTSXP || XLENGTH(st) != 1) {
        Rf_error("st must be a single integer.");
    }
    if(TYPEOF(en) != INTSXP || XLENGTH(en) != 1) {
        Rf_error("en must be a single integer.");
    }
    const char *ctg_str = CHAR(STRING_ELT(ctg, 0));
    int32_t st_val = INTEGER(st)[0];
    int32_t en_val = INTEGER(en)[0];
    cgranges_t *new_cr_ptr = cr_init();
    if (new_cr_ptr == NULL) {
        Rf_error("Failed to initialize cgranges_t object.");
    }
    int64_t *b = NULL;
    int64_t m_b = 0;
    int64_t n_overlap = cr_overlap(cr_ptr, ctg_str, st_val, en_val, &b, &m_b);
    for(int64_t i = 0; i < n_overlap; i++) {
        int64_t idx = b[i];
        cr_intv_t *r = &cr_ptr->r[idx];
        int32_t ctg_id = (int32_t)(r->x >> 32);
        const char *orig_ctg_name = cr_ptr->ctg[ctg_id].name;
        int32_t orig_st = cr_start(cr_ptr, idx);
        int32_t orig_en = cr_end(cr_ptr, idx);
        int32_t label = cr_label(cr_ptr, idx);
        cr_add(new_cr_ptr, orig_ctg_name, orig_st, orig_en, label);
    }
    free(b);
    cr_index(new_cr_ptr);
    SEXP new_cr_extptr = PROTECT(R_MakeExternalPtr(new_cr_ptr, R_NilValue, R_NilValue));
    R_RegisterCFinalizerEx(new_cr_extptr, (R_CFinalizer_t)RC_cr_destroy, TRUE);
    UNPROTECT(1);
    return new_cr_extptr;
}
// Register routines
static const R_CallMethodDef CallEntries[] = {
    {"RC_cr_init", (DL_FUNC) &RC_cr_init, 0},
    {"RC_cr_add", (DL_FUNC) &RC_cr_add, 5},
    {"RC_cr_index", (DL_FUNC) &RC_cr_index, 1},
    {"RC_cr_add_ctg", (DL_FUNC) &RC_cr_add_ctg, 2},
    {"RC_cr_get_ctg", (DL_FUNC) &RC_cr_get_ctg, 2},
    {"RC_cr_overlap", (DL_FUNC) &RC_cr_overlap, 4},
    {"RC_cr_overlap_vectorized", (DL_FUNC) &RC_cr_overlap_vectorized, 4},
    {"RC_cr_contain", (DL_FUNC) &RC_cr_contain, 3},
    {"RC_cr_contain_vectorized", (DL_FUNC) &RC_cr_contain_vectorized, 3},
    {"RC_get_cr_intervals", (DL_FUNC) &RC_get_cr_intervals, 2},
    {"RC_get_cr_contigs", (DL_FUNC) &RC_get_cr_contigs, 1},
    {"RC_get_cr_n_intervals", (DL_FUNC) &RC_get_cr_n_intervals, 1},
    {"RC_get_cr_n_contigs", (DL_FUNC) &RC_get_cr_n_contigs, 1},
    {"RC_cr_from_overlap", (DL_FUNC) &RC_cr_from_overlap, 3},
    {NULL, NULL, 0}
};

void R_init_RCGRanges(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, TRUE);
    R_forceSymbols(dll, TRUE);
}