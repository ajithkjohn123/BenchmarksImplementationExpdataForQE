;; MathSAT API call trace
;; generated on Mon Oct 10 14:21:33 2016
(set-option :global-decls true)
(set-option :config "verbosity=0")
(set-option :config "proof_generation=true")
(set-option :config "interpolation=true")
(set-option :config "model_generation=true")
(set-option :config "bool_model_generation=false")
(set-option :config "unsat_core_generation=0")
(set-option :config "random_seed=91648253")
(set-option :config "debug.dump_theory_lemmas=false")
(set-option :config "debug.solver_enabled=true")
(set-option :config "printer.bv_number_format=0")
(set-option :config "printer.fp_number_format=1")
(set-option :config "printer.defines_prefix=")
(set-option :config "preprocessor.toplevel_propagation=false")
(set-option :config "preprocessor.toplevel_propagation_limit=0")
(set-option :config "preprocessor.simplification=1")
(set-option :config "preprocessor.ite_minimization=false")
(set-option :config "preprocessor.ite_minimization_call_limit=150000")
(set-option :config "preprocessor.interpolation_ite_elimination=true")
(set-option :config "preprocessor.partial_nnf_conversion=false")
(set-option :config "dpll.restart_strategy=0")
(set-option :config "dpll.restart_initial=200")
(set-option :config "dpll.restart_geometric_factor=1.5")
(set-option :config "dpll.store_tlemmas=false")
(set-option :config "dpll.branching_random_frequency=0")
(set-option :config "dpll.branching_initial_phase=0")
(set-option :config "dpll.branching_cache_phase=2")
(set-option :config "dpll.branching_random_invalidate_phase_cache=false")
(set-option :config "dpll.branching_random_ignore_polarity=true")
(set-option :config "dpll.ghost_filtering=true")
(set-option :config "dpll.minimize_model=false")
(set-option :config "dpll.allsat_minimize_model=false")
(set-option :config "dpll.allsat_allow_duplicates=false")
(set-option :config "dpll.pop_btpoint_reset_var_order=false")
(set-option :config "dpll.glucose_var_activity=false")
(set-option :config "dpll.glucose_learnt_minimization=false")
(set-option :config "dpll.interpolation_mode=0")
(set-option :config "dpll.proof_simplification=false")
(set-option :config "dpll.preprocessor.mode=0")
(set-option :config "dpll.preprocessor.clause_size_limit=20")
(set-option :config "dpll.preprocessor.subsumption_size_limit=1000")
(set-option :config "dpll.preprocessor.elimination_grow_limit=0")
(set-option :config "dpll.preprocessor.elimination_phase=1")
(set-option :config "dpll.preprocessor.elimination_recent_vars_only=false")
(set-option :config "dpll.preprocessor.try_reelimination=true")
(set-option :config "theory.eq_propagation=true")
(set-option :config "theory.interface_eq_policy=2")
(set-option :config "theory.pairwise_interface_eq=true")
(set-option :config "theory.pure_literal_filtering=false")
(set-option :config "theory.euf.enabled=true")
(set-option :config "theory.euf.dyn_ack=0")
(set-option :config "theory.euf.dyn_ack_threshold=1")
(set-option :config "theory.euf.dyn_ack_limit=1000")
(set-option :config "theory.la.enabled=true")
(set-option :config "theory.la.split_rat_eq=false")
(set-option :config "theory.la.delay_alien=false")
(set-option :config "theory.la.pivoting_greedy_threshold=0")
(set-option :config "theory.la.lazy_expl_threshold=10")
(set-option :config "theory.la.interpolation_laz_use_floor=true")
(set-option :config "theory.la.pure_equality_filtering=true")
(set-option :config "theory.la.detect_euf_fragment=false")
(set-option :config "theory.la.deduction_enabled=true")
(set-option :config "theory.la.laz_equality_elimination=true")
(set-option :config "theory.la.laz_internal_branch_and_bound=true")
(set-option :config "theory.la.laz_internal_branch_and_bound_limit=1")
(set-option :config "theory.la.laz_cuts_from_proofs_mode=3")
(set-option :config "theory.la.laz_enabled=true")
(set-option :config "theory.la.laz_unit_cube_test=true")
(set-option :config "theory.la.interpolation_mode=0")
(set-option :config "theory.bv.enabled=true")
(set-option :config "theory.bv.div_by_zero_mode=0")
(set-option :config "theory.bv.delay_propagated_eqs=true")
(set-option :config "theory.bv.eager=false")
(set-option :config "theory.bv.bit_blast_mode=0")
(set-option :config "theory.bv.interpolation_mode=0")
(set-option :config "theory.bv.proof_simplification=false")
(set-option :config "theory.bv.lazydpll.restart_strategy=0")
(set-option :config "theory.bv.lazydpll.restart_initial=200")
(set-option :config "theory.bv.lazydpll.restart_geometric_factor=1.5")
(set-option :config "theory.bv.lazydpll.branching_random_frequency=0")
(set-option :config "theory.bv.lazydpll.branching_initial_phase=0")
(set-option :config "theory.bv.lazydpll.branching_cache_phase=2")
(set-option :config "theory.bv.lazydpll.branching_random_invalidate_phase_cache=false")
(set-option :config "theory.bv.lazydpll.branching_random_ignore_polarity=true")
(set-option :config "theory.bv.lazydpll.ghost_filtering=false")
(set-option :config "theory.bv.lazydpll.glucose_var_activity=false")
(set-option :config "theory.bv.lazydpll.glucose_learnt_minimization=false")
(set-option :config "theory.arr.enabled=true")
(set-option :config "theory.arr.mode=0")
(set-option :config "theory.arr.lazy_lemmas=true")
(set-option :config "theory.arr.check_on_approx=false")
(set-option :config "theory.arr.auto_wr_lemma=true")
(set-option :config "theory.arr.enable_wr_lemmas=true")
(set-option :config "theory.arr.enable_row_lemmas=true")
(set-option :config "theory.arr.enable_ext_lemmas=true")
(set-option :config "theory.arr.max_wr_lemmas=0")
(set-option :config "theory.arr.max_row_lemmas=0")
(set-option :config "theory.arr.max_ext_lemmas=0")
(set-option :config "theory.arr.enable_ext_polarity=true")
(set-option :config "theory.arr.enable_ext_arg=true")
(set-option :config "theory.arr.enable_witness=false")
(set-option :config "theory.arr.enable_nonlinear=true")
(set-option :config "theory.arr.enable_nonlinear_strat=false")
(set-option :config "theory.arr.enable_foreign=false")
(set-option :config "theory.arr.permanent_lemma_inst=false")
(set-option :config "theory.fp.enabled=true")
(set-option :config "theory.fp.minmax_zero_mode=1")
(set-option :config "theory.fp.mode=1")
(set-option :config "theory.fp.bv_combination_enabled=true")
(set-option :config "theory.fp.bit_blast_mode=2")
(set-option :config "theory.fp.dpll.restart_strategy=3")
(set-option :config "theory.fp.dpll.restart_initial=200")
(set-option :config "theory.fp.dpll.restart_geometric_factor=1.5")
(set-option :config "theory.fp.dpll.branching_random_frequency=0.01")
(set-option :config "theory.fp.dpll.branching_initial_phase=0")
(set-option :config "theory.fp.dpll.branching_cache_phase=1")
(set-option :config "theory.fp.dpll.branching_random_invalidate_phase_cache=false")
(set-option :config "theory.fp.dpll.branching_random_ignore_polarity=true")
(set-option :config "theory.fp.dpll.ghost_filtering=false")
(set-option :config "theory.fp.dpll.glucose_var_activity=true")
(set-option :config "theory.fp.dpll.glucose_learnt_minimization=true")
(set-option :config "theory.fp.acdcl_conflict_generalization=2")
(set-option :config "theory.fp.interpolation_mode=0")
(set-option :config "theory.fp.acdcl_single_propagation_limit=0")
(set-option :config "theory.fp.acdcl_single_generalization_limit=0")
(set-option :config "theory.fp.acdcl_generalization_fair=true")

(reset-assertions)
(declare-fun A_1 () (_ BitVec 16))
(declare-fun A_2 () (_ BitVec 16))
(declare-fun COUNT_1 () (_ BitVec 8))
(declare-fun COUNT_2 () (_ BitVec 8))
(declare-fun COUNT_3 () (_ BitVec 8))
(declare-fun FDBCKSUM_0 () (_ BitVec 16))
(declare-fun RESET_1 () (_ BitVec 1))
(declare-fun RESET_2 () (_ BitVec 1))
(declare-fun RESET_3 () (_ BitVec 1))
(declare-fun RESET_4 () (_ BitVec 1))
(declare-fun RNGCHNGFLG_2 () (_ BitVec 1))
(declare-fun RNGCHNGFLG_3 () (_ BitVec 1))
(declare-fun RNGCHNGFLG_4 () (_ BitVec 1))
(declare-fun SUM128L_1 () (_ BitVec 16))
(declare-fun SUM128L_0 () (_ BitVec 16))
(declare-fun SUM128UL_2 () (_ BitVec 16))
(declare-fun VLDEOC_1 () (_ BitVec 1))
(declare-fun VLDEOC_2 () (_ BitVec 1))
(declare-fun VLDEOC_3 () (_ BitVec 1))
(declare-fun VLDEOC_4 () (_ BitVec 1))
(declare-fun autoname_1 () (_ BitVec 1))
(define-fun $e1 () Bool (= VLDEOC_3 VLDEOC_4))
(define-fun $e2 () Bool (= VLDEOC_2 VLDEOC_3))
(define-fun $e3 () Bool (and (= RESET_2 RESET_3) (= RNGCHNGFLG_2 RNGCHNGFLG_3) $e2))
(define-fun $e4 () Bool (= RESET_2 #b1))
(define-fun $e5 () Bool (= COUNT_2 #b00000000))
(define-fun $e6 () Bool (= FDBCKSUM_0 A_1))
(define-fun $e7 () Bool (= A_1 #b0000000000000000))
(define-fun $e8 () Bool (and $e7 $e6))
(define-fun $e9 () Bool (and $e8 $e5))
(define-fun $e10 () Bool (= RNGCHNGFLG_2 #b1))
(define-fun $e11 () Bool (and (= VLDEOC_2 #b1) (not $e2)))
(define-fun $e12 () Bool (bvult #b10000000 COUNT_2))
(define-fun $e13 () Bool (= A_1 SUM128UL_2))
(define-fun $e14 () Bool (and $e13 $e6))
(define-fun $e15 () Bool (= A_1 A_2))
(define-fun $e16 () Bool (and $e15 $e6))
(define-fun $e17 () Bool (and $e16 $e5))
(define-fun $e18 () Bool (ite (not $e3) (ite $e4 $e9 (ite $e10 $e9 (ite $e11 (ite $e12 $e9 (and $e14 $e5)) $e17))) $e17))
(define-fun $e19 () Bool (= COUNT_2 (bvadd COUNT_3 #b00000001)))
(define-fun $e20 () Bool (and $e8 $e19))
(define-fun $e21 () Bool (and $e16 $e19))
(define-fun $e22 () Bool (= COUNT_2 COUNT_3))
(define-fun $e23 () Bool (and $e8 $e22))
(define-fun $e24 () Bool (and $e16 $e22))
(define-fun $e25 () Bool (ite (not $e3) (ite $e4 $e23 (ite $e10 $e23 (ite $e11 (ite $e12 $e23 (and $e14 $e22)) $e24))) $e24))
(define-fun $e26 () Bool (= VLDEOC_1 VLDEOC_2))
(define-fun $e27 () Bool (and (= RESET_1 RESET_2) $e26))
(define-fun $e28 () Bool (= RESET_1 #b1))
(define-fun $e29 () Bool (= COUNT_1 #b00000000))
(define-fun $e30 () Bool (= SUM128L_0 #b0000000000000000))
(define-fun $e31 () Bool (and $e30 $e7))
(define-fun $e32 () Bool (and $e31 $e29))
(define-fun $e33 () Bool (and (= VLDEOC_1 #b1) (not $e26)))
(define-fun $e34 () Bool (bvult #b10000000 COUNT_1))
(define-fun $e35 () Bool (= autoname_1 #b1))
(define-fun $e36 () Bool (= SUM128L_0 A_1))
(define-fun $e37 () Bool (and $e36 $e7))
(define-fun $e38 () Bool (= SUM128L_0 SUM128L_1))
(define-fun $e39 () Bool (and $e38 $e7))
(define-fun $e40 () Bool (and $e39 $e29))
(define-fun $e41 () Bool (ite (not $e27) (ite $e28 $e32 (ite $e33 (ite $e34 (ite $e35 $e32 (and $e37 $e29)) $e40) $e40)) $e40))
(define-fun $e42 () Bool (and $e30 $e13))
(define-fun $e43 () Bool (and $e42 $e29))
(define-fun $e44 () Bool (and $e36 $e13))
(define-fun $e45 () Bool (and $e38 $e13))
(define-fun $e46 () Bool (and $e45 $e29))
(define-fun $e47 () Bool (and $e30 $e15))
(define-fun $e48 () Bool (and $e47 $e29))
(define-fun $e49 () Bool (and $e36 $e15))
(define-fun $e50 () Bool (and $e38 $e15))
(define-fun $e51 () Bool (and $e50 $e29))
(define-fun $e52 () Bool (ite (not $e27) (ite $e28 $e48 (ite $e33 (ite $e34 (ite $e35 $e48 (and $e49 $e29)) $e51) $e51)) $e51))
(define-fun $e53 () Bool (ite (not $e3) (ite $e4 $e41 (ite $e10 $e41 (ite $e11 (ite $e12 $e41 (ite (not $e27) (ite $e28 $e43 (ite $e33 (ite $e34 (ite $e35 $e43 (and $e44 $e29)) $e46) $e46)) $e46)) $e52))) $e52))
(define-fun $e54 () Bool (= COUNT_1 (bvadd COUNT_2 #b00000001)))
(define-fun $e55 () Bool (and $e31 $e54))
(define-fun $e56 () Bool (and $e39 $e54))
(define-fun $e57 () Bool (ite (not $e27) (ite $e28 $e55 (ite $e33 (ite $e34 (ite $e35 $e55 (and $e37 $e54)) $e56) $e56)) $e56))
(define-fun $e58 () Bool (and $e42 $e54))
(define-fun $e59 () Bool (and $e45 $e54))
(define-fun $e60 () Bool (and $e47 $e54))
(define-fun $e61 () Bool (and $e50 $e54))
(define-fun $e62 () Bool (ite (not $e27) (ite $e28 $e60 (ite $e33 (ite $e34 (ite $e35 $e60 (and $e49 $e54)) $e61) $e61)) $e61))
(define-fun $e63 () Bool (= COUNT_1 COUNT_2))
(define-fun $e64 () Bool (and $e31 $e63))
(define-fun $e65 () Bool (and $e39 $e63))
(define-fun $e66 () Bool (ite (not $e27) (ite $e28 $e64 (ite $e33 (ite $e34 (ite $e35 $e64 (and $e37 $e63)) $e65) $e65)) $e65))
(define-fun $e67 () Bool (and $e42 $e63))
(define-fun $e68 () Bool (and $e45 $e63))
(define-fun $e69 () Bool (and $e47 $e63))
(define-fun $e70 () Bool (and $e50 $e63))
(define-fun $e71 () Bool (ite (not $e27) (ite $e28 $e69 (ite $e33 (ite $e34 (ite $e35 $e69 (and $e49 $e63)) $e70) $e70)) $e70))
(define-fun $e72 () Bool (ite (not $e3) (ite $e4 $e66 (ite $e10 $e66 (ite $e11 (ite $e12 $e66 (ite (not $e27) (ite $e28 $e67 (ite $e33 (ite $e34 (ite $e35 $e67 (and $e44 $e63)) $e68) $e68)) $e68)) $e71))) $e71))
(define-fun .f1 () Bool (ite (not $e3) (ite $e4 $e53 (ite $e10 $e53 (ite $e11 (ite $e12 $e53 (ite (not $e3) (ite $e4 $e57 (ite $e10 $e57 (ite $e11 (ite $e12 $e57 (ite (not $e27) (ite $e28 $e58 (ite $e33 (ite $e34 (ite $e35 $e58 (and $e44 $e54)) $e59) $e59)) $e59)) $e62))) $e62)) $e72))) $e72))
(define-fun .f2 () Bool (ite (not (and (= RESET_3 RESET_4) (= RNGCHNGFLG_3 RNGCHNGFLG_4) $e1)) (ite (= RESET_3 #b1) $e18 (ite (= RNGCHNGFLG_3 #b1) $e18 (ite (and (= VLDEOC_3 #b1) (not $e1)) (ite (bvult #b10000000 COUNT_3) $e18 (ite (not $e3) (ite $e4 $e20 (ite $e10 $e20 (ite $e11 (ite $e12 $e20 (and $e14 $e19)) $e21))) $e21)) $e25))) $e25))
(assert (! .f1 :interpolation-group .g0))
(assert (! .f2 :interpolation-group .g0))

(define-fun $f1 () Bool (= (bvadd RNGCHNGFLG_2 RNGCHNGFLG_3) #b0))
(define-fun $f2 () Bool (= (bvadd VLDEOC_3 VLDEOC_4) #b0))
(define-fun $f3 () Bool (= (bvadd RESET_2 RESET_3) #b0))
(define-fun $f4 () Bool (= (bvadd VLDEOC_2 VLDEOC_3) #b0))
(define-fun $f5 () Bool (and $f2 $f1 $f3 $f4))
(define-fun $f6 () Bool (= (bvadd VLDEOC_1 VLDEOC_2) #b0))
(define-fun $f7 () Bool (and $f6 $f5))
(define-fun $f8 () Bool (= (bvadd RESET_1 RESET_2) #b0))
(define-fun $f9 () Bool (= (bvadd SUM128L_0 (bvmul #b1111111111111111 SUM128L_1)) #b0000000000000000))
(define-fun $f10 () Bool (= (bvadd RNGCHNGFLG_3 RNGCHNGFLG_4) #b0))
(define-fun $f11 () Bool (= (bvadd RESET_3 RESET_4) #b0))
(define-fun $f12 () Bool (= VLDEOC_2 #b1))
(define-fun $f13 () Bool (= RESET_2 #b1))
(define-fun $f14 () Bool (= RNGCHNGFLG_2 #b1))
(define-fun $f15 () Bool (and $f12 $f2))
(define-fun $f16 () Bool (= FDBCKSUM_0 #b0000000000000000))
(define-fun $f17 () Bool (= VLDEOC_1 #b1))
(define-fun $f18 () Bool (and $f17 $f2 $f9 (not $f12)))
(define-fun $f19 () Bool (= RESET_1 #b1))
(define-fun $f20 () Bool (and $f9 $f5 (not $f19)))
(define-fun $f21 () Bool (and $f14 $f2))
(define-fun $f22 () Bool (and $f6 $f21 $f8 $f9))
(define-fun $f23 () Bool (and $f17 $f21 $f9))
(define-fun $f24 () Bool (and $f9 $f21))
(define-fun $f25 () Bool (and $f13 $f2))
(define-fun $f26 () Bool (= SUM128L_0 #b0000000000000000))
(define-fun $f27 () Bool (and $f19 $f25 $f26))
(define-fun $f28 () Bool (and $f19 $f5 $f26))
(define-fun $f29 () Bool (and $f6 $f25))
(define-fun $f30 () Bool (and $f8 $f29 $f9))
(define-fun $f31 () Bool (and $f9 $f29))
(define-fun $f32 () Bool (and $f17 $f25 $f9))
(define-fun $f33 () Bool (and $f9 $f25))
(define-fun $f34 () Bool (and $f19 $f2 $f26 (not $f12)))
(define-fun $f35 () Bool (and $f19 $f21 $f26))
(define-fun $f36 () Bool (and $f4 $f2))
(define-fun $f37 () Bool (and $f19 $f36 $f26 (not $f13) (not $f14) $f10 $f11 (not $f8)))
(define-fun $f38 () Bool (and (not $f13) $f35 $f10 $f11 (not $f8)))
(define-fun $f39 () Bool (and (not $f6) $f27 $f10 $f11))
(define-fun $f40 () Bool (and (not $f19) $f33 (not $f17) $f10 $f11 (not $f8)))
(define-fun $f41 () Bool (and $f10 $f30 $f11))
(define-fun $f42 () Bool (and $f6 $f36 $f8 $f9 (not $f13) (not $f14) $f10 $f11))
(define-fun $f43 () Bool (and (not $f19) $f32 (not $f6) $f10 $f11 (not $f8)))
(define-fun $f44 () Bool (and (not $f13) $f23 (not $f19) (not $f6) $f10 $f11))
(define-fun $f45 () Bool (and (not $f13) $f22 $f10 $f11))
(define-fun $f46 () Bool (and (not $f13) $f24 (not $f19) (not $f6) (not $f17) $f10 $f11))
(define-fun $f47 () Bool (and $f9 $f36 (not $f13) (not $f14) (not $f19) (not $f6) (not $f17) $f10 $f11))
(define-fun $f48 () Bool (and (not $f19) $f31 $f10 $f11 (not $f8)))
(define-fun $f49 () Bool (and (not $f13) $f18 (not $f14) (not $f19) (not $f6) $f10 $f11))
(define-fun $f50 () Bool (= RESET_3 #b1))
(define-fun $f51 () Bool (and $f14 $f50))
(define-fun $f52 () Bool (and $f4 $f50))
(define-fun $f53 () Bool (= VLDEOC_3 #b1))
(define-fun $f54 () Bool (= RNGCHNGFLG_3 #b1))
(define-fun $f55 () Bool (and $f3 $f1))
(define-fun $f56 () Bool (and $f4 $f55))
(define-fun $f57 () Bool (and $f54 $f56))
(define-fun $f58 () Bool (and $f6 $f50 $f8 $f9 (not $f12)))
(define-fun $f59 () Bool (and $f6 $f54 $f8 $f9 (not $f12)))
(define-fun $f60 () Bool (and $f54 $f13))
(define-fun $f61 () Bool (and $f13 $f50))
(define-fun $f62 () Bool (and $f50 $f1 $f3 $f4))
(define-fun $f63 () Bool (and $f19 $f61 $f26))
(define-fun $f64 () Bool (= (bvadd FDBCKSUM_0 (bvmul #b1111111111111111 SUM128UL_2)) #b0000000000000000))
(define-fun $f65 () Bool (ite $f16 $f64 false))
(define-fun $f66 () Bool (ite $f9 $f65 false))
(define-fun $f67 () Bool (ite $f19 (ite $f26 $f65 false) $f66))
(define-fun $f68 () Bool (ite $f9 $f16 false))
(define-fun $f69 () Bool (ite $f19 (ite $f26 $f16 false) $f68))
(define-fun $f70 () Bool (ite $f6 (ite $f8 $f68 $f69) $f69))
(define-fun $f71 () Bool (ite $f16 false $f64))
(define-fun $f72 () Bool (ite $f9 $f71 false))
(define-fun $f73 () Bool (ite $f19 (ite $f26 $f71 false) $f72))
(define-fun $f74 () Bool (ite $f9 (not $f16) false))
(define-fun $f75 () Bool (ite $f19 (ite $f26 (not $f16) false) $f74))
(define-fun $f76 () Bool (and (not $f4) $f55 (not $f13) $f10 $f11 (not $f2) (not $f50)))
(define-fun $f77 () Bool (ite $f16 true $f64))
(define-fun $f78 () Bool (ite $f9 $f77 false))
(define-fun $f79 () Bool (ite $f26 $f77 false))
(define-fun $f80 () Bool (ite $f9 $f77 $f64))
(define-fun $f81 () Bool (= (bvadd FDBCKSUM_0 (bvmul #b1111111111111111 SUM128L_0)) #b0000000000000000))
(define-fun $f82 () Bool (ite $f6 (ite $f8 $f78 (ite $f19 $f79 $f78)) (ite $f19 $f79 (ite $f26 (ite $f17 $f80 $f78) (ite $f17 (ite $f81 $f80 $f78) $f78)))))
(define-fun $f83 () Bool (ite $f19 $f26 $f9))
(define-fun $f84 () Bool (ite $f8 $f9 $f83))
(define-fun $f85 () Bool (ite $f6 $f84 (ite $f19 $f26 (ite $f26 (ite $f17 true $f9) (ite $f17 (ite $f81 true $f9) $f9)))))
(define-fun $f86 () Bool (ite $f6 $f84 $f83))
(define-fun $f87 () Bool (ite $f54 $f86 $f85))
(define-fun $f88 () Bool (ite $f9 $f64 false))
(define-fun $f89 () Bool (ite $f19 (ite $f26 $f64 false) $f88))
(define-fun $f90 () Bool (ite $f6 (ite $f8 $f88 $f89) $f89))
(define-fun $f91 () Bool (ite $f13 $f70 (ite $f14 $f70 (ite $f12 (ite $f54 $f90 $f82) $f87))))
(define-fun $f92 () Bool (ite $f4 (ite $f13 $f70 (ite $f14 $f70 $f87)) $f91))
(define-fun $f93 () Bool (ite $f13 $f70 (ite $f14 $f70 (ite $f12 $f90 $f86))))
(define-fun $f94 () Bool (ite $f4 (ite $f13 $f70 (ite $f14 $f70 $f86)) $f93))
(define-fun $f95 () Bool (ite $f1 (ite $f3 (ite $f4 $f86 $f93) $f94) $f94))
(define-fun $f96 () Bool (ite $f13 $f70 (ite $f14 $f70 (ite $f12 $f82 $f85))))
(define-fun $f97 () Bool (ite $f4 (ite $f13 $f70 (ite $f14 $f70 $f85)) $f96))
(define-fun $f98 () Bool (ite $f50 $f95 (ite $f1 (ite $f3 (ite $f4 $f87 $f91) $f92) $f92)))
(define-fun .f3 () Bool (not (not (and (not (and $f8 $f7 $f9 $f10 $f11)) (not (and $f6 $f2 $f8 $f9 (not $f12) (not $f4) (not $f13) (not $f14) $f10 $f11)) (not (and $f6 $f15 $f8 $f9 (not $f4) (not $f13) (not $f14) $f10 $f11 $f16)) (not (and (not $f4) $f18 (not $f13) (not $f14) (not $f19) (not $f6) $f10 $f11)) (not (and $f17 $f5 $f9 (not $f19) (not $f6) $f10 $f11)) (not (and (not $f6) $f20 (not $f17) $f10 $f11)) (not (and (not $f4) $f22 (not $f13) $f10 $f11 $f16)) (not (and (not $f4) $f23 (not $f13) (not $f19) (not $f6) $f10 $f11 $f16)) (not (and (not $f4) $f24 (not $f13) (not $f19) (not $f6) (not $f17) $f10 $f11 $f16)) (not (and $f9 $f15 (not $f4) (not $f13) (not $f14) (not $f19) (not $f6) (not $f17) $f10 $f11 $f16)) (not (and (not $f4) $f27 (not $f6) $f10 $f11 $f16)) (not (and (not $f6) $f28 $f10 $f11)) (not (and (not $f4) $f30 $f10 $f11 $f16)) (not (and (not $f4) $f31 (not $f19) $f10 $f11 (not $f8) $f16)) (not (and $f10 $f28 $f11 (not $f8))) (not (and (not $f4) $f32 (not $f19) (not $f6) $f10 $f11 (not $f8) $f16)) (not (and (not $f17) $f20 $f10 $f11 (not $f8))) (not (and $f9 $f7 (not $f19) $f10 $f11 (not $f8))) (not (and (not $f4) $f33 (not $f19) (not $f17) $f10 $f11 (not $f8) $f16)) (not (and (not $f4) $f34 (not $f13) (not $f14) $f10 $f11 (not $f8))) (not (and $f19 $f15 $f26 (not $f4) (not $f13) (not $f14) $f10 $f11 (not $f8) $f16)) (not (and (not $f4) $f35 (not $f13) $f10 $f11 (not $f8) $f16)) (not (and (not $f1) $f37)) (not (and (not $f1) $f38 $f16)) (not (and (not $f1) $f39 $f16)) (not (and (not $f1) $f40 $f16)) (not (and (not $f1) $f41 $f16)) (not (and (not $f1) $f42)) (not (and (not $f1) $f43 $f16)) (not (and (not $f1) $f44 $f16)) (not (and (not $f1) $f45 $f16)) (not (and (not $f1) $f46 $f16)) (not (and (not $f1) $f47)) (not (and (not $f1) $f48 $f16)) (not (and (not $f1) $f49)) (not (and (not $f3) $f44 $f16)) (not (and (not $f3) $f46 $f16)) (not (and (not $f13) $f34 (not $f14) $f10 $f11 (not $f8) (not $f3))) (not (and (not $f3) $f45 $f16)) (not (and (not $f3) $f42)) (not (and (not $f3) $f41 $f16)) (not (and (not $f3) $f49)) (not (and (not $f3) $f39 $f16)) (not (and (not $f3) $f47)) (not (and (not $f3) $f37)) (not (and (not $f3) $f43 $f16)) (not (and (not $f3) $f40 $f16)) (not (and (not $f3) $f48 $f16)) (not (and (not $f3) $f38 $f16)) (not (and $f19 $f51 $f26 (not $f13) (not $f8) (not $f3) (not $f2) $f16)) (not (and $f9 $f52 (not $f13) (not $f14) (not $f19) (not $f6) (not $f17) (not $f3) (not $f2))) (not (and $f6 $f51 $f8 $f9 (not $f13) (not $f3) (not $f2) $f16)) (not (and $f6 $f12 $f8 $f9 (not $f4) (not $f13) (not $f14) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f19 $f57 $f26 (not $f8) (not $f2) (not $f50))) (not (and (not $f4) $f58 (not $f13) (not $f14) (not $f2))) (not (and (not $f4) $f59 (not $f13) (not $f14) (not $f2) (not $f50))) (not (and (not $f13) $f59 (not $f14) (not $f1) (not $f2) (not $f50))) (not (and $f6 $f56 $f8 $f9 (not $f2) (not $f53) (not $f54) (not $f50))) (not (and $f19 $f14 $f26 (not $f13) (not $f8) (not $f1) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f54 $f4 $f19 $f26 (not $f13) (not $f14) (not $f8) (not $f1) (not $f2) (not $f50))) (not (and $f19 $f60 $f26 (not $f6) (not $f3) (not $f2) (not $f50) $f16)) (not (and $f19 $f56 $f26 (not $f8) (not $f2) (not $f53) (not $f54) (not $f50))) (not (and $f17 $f60 $f9 (not $f19) (not $f6) (not $f8) (not $f3) (not $f2) (not $f50) $f16)) (not (and $f17 $f13 $f9 (not $f19) (not $f6) (not $f8) (not $f3) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f17 $f51 $f9 (not $f13) (not $f19) (not $f6) (not $f3) (not $f2) $f16)) (not (and $f17 $f61 $f9 (not $f19) (not $f6) (not $f8) (not $f1) (not $f2) $f16)) (not (and $f19 $f13 $f26 (not $f6) (not $f3) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f17 $f57 $f9 (not $f19) (not $f6) (not $f2) (not $f50))) (not (and $f17 $f14 $f9 (not $f13) (not $f19) (not $f6) (not $f1) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f9 $f51 (not $f13) (not $f19) (not $f6) (not $f17) (not $f3) (not $f2) $f16)) (not (and $f9 $f57 (not $f19) (not $f6) (not $f17) (not $f2) (not $f50))) (not (and $f19 $f52 $f26 (not $f13) (not $f14) (not $f8) (not $f3) (not $f2))) (not (and $f9 $f60 (not $f19) (not $f17) (not $f8) (not $f3) (not $f2) (not $f50) $f16)) (not (and (not $f13) $f58 (not $f14) (not $f3) (not $f2))) (not (and $f17 $f62 $f9 (not $f19) (not $f6) (not $f8) (not $f2))) (not (and $f19 $f50 $f26 (not $f12) (not $f4) (not $f13) (not $f14) (not $f8) (not $f2))) (not (and $f17 $f50 $f9 (not $f12) (not $f13) (not $f14) (not $f19) (not $f6) (not $f3) (not $f2))) (not (and (not $f6) $f63 (not $f1) (not $f2) $f16)) (not (and $f53 $f14 $f19 $f26 (not $f13) (not $f8) (not $f1) (not $f2) (not $f54) (not $f50) $f16)) (not (and $f54 $f14 $f19 $f26 (not $f4) (not $f13) (not $f8) (not $f2) (not $f50) $f16)) (not (and $f9 $f62 (not $f19) (not $f17) (not $f8) (not $f2))) (not (and $f9 $f13 (not $f4) (not $f19) (not $f17) (not $f8) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f19 $f62 $f26 (not $f6) (not $f2))) (not (and $f19 $f12 $f26 (not $f4) (not $f13) (not $f14) (not $f8) (not $f2) (not $f53) (not $f54) (not $f50) $f16)) (not (and $f19 $f54 $f26 (not $f12) (not $f4) (not $f13) (not $f14) (not $f8) (not $f2) (not $f50))) (not (and $f53 $f56 $f19 $f26 (not $f8) (not $f2) (not $f54) (not $f50))) (not (and (not $f4) $f63 (not $f6) (not $f2) $f16)) (not (and $f12 $f55 (not $f4) (not $f13) (not $f14) $f10 $f11 (not $f2) (not $f53) (not $f54) (not $f50) (ite $f6 (ite $f8 $f66 $f67) $f67))) (not (and (not $f12) $f55 (not $f4) (not $f13) (not $f14) $f10 $f11 (not $f2) (not $f50) $f70)) (not (and $f14 $f55 (not $f4) (not $f13) $f10 $f11 (not $f2) (not $f50) $f70)) (not (and $f76 (ite $f14 false (ite $f12 (ite $f6 (ite $f8 $f72 $f73) $f73) (ite $f6 (ite $f8 $f74 $f75) $f75))))) (not (and $f76 (ite $f14 (ite $f54 false $f70) (ite $f12 (ite $f54 false $f82) (ite $f54 false $f85))))) (not (and $f10 $f56 $f11 (not $f2) (not $f50) $f87)) (not (and $f10 $f1 $f11 (not $f3) (not $f2) (not $f50) $f92)) (not (and $f11 $f10 (not $f1) (not $f2) (not $f50) $f92)) (not (and $f10 $f50 $f11 (not $f2) $f95)) (not (and $f10 $f2 $f11 (ite $f1 (ite $f3 (ite $f4 $f85 $f96) $f97) $f97))) (not (and $f10 (not $f11) $f98)) (not (and (not $f10) $f98)) true))))

(assert (! .f3 :interpolation-group .g1))

(declare-fun next_FDBCKSUM_0 () (_ BitVec 16))
(declare-fun next_SUM128L_1 () (_ BitVec 16))
(declare-fun next_SUM128L_0 () (_ BitVec 16))
(declare-fun next_SUM128UL_2 () (_ BitVec 16))
(declare-fun next_VLDEOC_1 () (_ BitVec 1))
(declare-fun next_VLDEOC_2 () (_ BitVec 1))
(declare-fun next_VLDEOC_3 () (_ BitVec 1))
(declare-fun next_VLDEOC_4 () (_ BitVec 1))
(define-fun .f4 () Bool (not (= VLDEOC_4 next_VLDEOC_4)))
(define-fun .f5 () Bool (not (= VLDEOC_3 next_VLDEOC_3)))
(define-fun .f6 () Bool (not (= VLDEOC_2 next_VLDEOC_2)))
(define-fun .f7 () Bool (not (= VLDEOC_1 next_VLDEOC_1)))
(define-fun .f8 () Bool (not (= SUM128UL_2 next_SUM128UL_2)))
(define-fun .f9 () Bool (not (= SUM128L_0 next_SUM128L_0)))
(define-fun .f10 () Bool (not (= SUM128L_1 next_SUM128L_1)))
(define-fun .f11 () Bool (bvult next_FDBCKSUM_0 FDBCKSUM_0))

(assert (! .f4 :interpolation-group .g1))
(assert (! .f5 :interpolation-group .g1))
(assert (! .f6 :interpolation-group .g1))
(assert (! .f7 :interpolation-group .g1))
(assert (! .f8 :interpolation-group .g1))
(assert (! .f9 :interpolation-group .g1))
(assert (! .f10 :interpolation-group .g1))
(assert (! .f11 :interpolation-group .g1))

(check-sat)
(get-interpolant (.g0))