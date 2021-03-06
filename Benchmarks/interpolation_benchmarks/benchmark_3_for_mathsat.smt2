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
(declare-fun clock_1 () (_ BitVec 1))
(declare-fun clock_2 () (_ BitVec 1))
(declare-fun clock_3 () (_ BitVec 1))
(declare-fun cont1_1 () (_ BitVec 8))
(declare-fun cont1_2 () (_ BitVec 8))
(declare-fun cont_1 () (_ BitVec 8))
(declare-fun cont_2 () (_ BitVec 8))
(declare-fun r_in_1 () (_ BitVec 8))
(declare-fun r_in_2 () (_ BitVec 8))
(declare-fun reset_1 () (_ BitVec 1))
(declare-fun reset_2 () (_ BitVec 1))
(declare-fun reset_3 () (_ BitVec 1))
(declare-fun stato_1 () (_ BitVec 4))
(declare-fun stato_2 () (_ BitVec 4))
(declare-fun stbi_2 () (_ BitVec 1))
(declare-fun x_in_2 () (_ BitVec 8))
(declare-fun x_out_1 () (_ BitVec 8))
(declare-fun x_out_0 () (_ BitVec 8))
(define-fun $e1 () Bool (= clock_1 clock_2))
(define-fun $e2 () Bool (and (= reset_1 reset_2) $e1))
(define-fun $e3 () Bool (= x_out_0 #b00000000))
(define-fun $e4 () (_ BitVec 1) (bvand clock_1 (bvnot (ite $e1 #b1 #b0))))
(define-fun $e5 () Bool (= stato_1 #b0000))
(define-fun $e6 () Bool (= stato_1 #b0001))
(define-fun $e7 () Bool (= stato_1 #b0010))
(define-fun $e8 () Bool (= x_out_1 x_out_0))
(define-fun $e9 () Bool (= clock_2 clock_3))
(define-fun $e10 () Bool (and (= reset_2 reset_3) $e9))
(define-fun $e11 () (_ BitVec 1) (bvand clock_2 (bvnot (ite $e9 #b1 #b0))))
(define-fun $e12 () Bool (= stato_2 #b0000))
(define-fun $e13 () Bool (= cont1_1 cont1_2))
(define-fun $e14 () Bool (= stato_2 #b0001))
(define-fun $e15 () Bool (= stato_2 #b0010))
(define-fun $e16 () Bool (and (not (= r_in_2 #b01100011)) (not (= r_in_2 #b00000000))))
(define-fun $e17 () Bool (= stato_2 #b0011))
(define-fun $e18 () Bool (= r_in_2 #b00000001))
(define-fun $e19 () Bool (= stato_2 #b0100))
(define-fun $e20 () Bool (= stato_2 #b0101))
(define-fun $e21 () Bool (bvult #b00100110 cont1_2))
(define-fun $e22 () Bool (= cont1_1 (bvadd cont1_2 #b00100110)))
(define-fun $e23 () Bool (= stato_2 #b0110))
(define-fun $e24 () Bool (bvult #b01100011 cont1_2))
(define-fun $e25 () Bool (= cont_1 #b00000000))
(define-fun $e26 () Bool (= cont_1 cont_2))
(define-fun $e27 () Bool (bvult #b00100101 cont_2))
(define-fun $e28 () Bool (= r_in_1 x_in_2))
(define-fun $e29 () Bool (= r_in_1 r_in_2))
(define-fun $e30 () Bool (bvult #b00100110 r_in_2))
(define-fun $e31 () Bool (= stato_1 #b0101))
(define-fun $e32 () Bool (= stato_1 #b0110))
(define-fun $e33 () Bool (= stato_1 #b0111))
(define-fun $e34 () Bool (= stato_1 stato_2))
(define-fun .f1 () Bool (not (and (not $e34) $e10)))
(assert (! .f1 :interpolation-group .g0))
(define-fun .f2 () Bool (distinct (bvnot (bvand (bvnot (ite $e10 #b1 #b0)) (bvnot (bvand (bvnot (bvand reset_2 (bvnot (ite $e5 #b1 #b0)))) (bvnot (bvand (bvnot reset_2) (bvnot (bvand (bvnot (bvand $e11 (bvnot (bvand (bvnot (ite (and $e12 (not $e6)) #b1 #b0)) (bvnot (bvand (bvnot (ite $e12 #b1 #b0)) (bvnot (bvand (bvnot (bvand (ite $e14 #b1 #b0) (bvnot (bvand (bvnot (bvand stbi_2 (bvnot (ite $e6 #b1 #b0)))) (bvnot (bvand (bvnot stbi_2) (bvnot (ite $e7 #b1 #b0)))))))) (bvnot (ite (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not $e34) (not $e23))) (not (and (not (and (not (and (not $e33) (not $e24))) (not (and (not $e32) $e24)))) $e23)))) (not $e20))) (not (and (not (and (not (and (not $e33) (not $e21))) (not (and (not $e31) $e21)))) $e20)))) (not $e19))) (not (and (not (and (not (and (not $e32) (not $e18))) (not (and (not $e31) $e18)))) $e19)))) (not $e17))) (not (and (not (= stato_1 #b0100)) $e17)))) (not $e15))) (not (and (not (and (not (and (not (= stato_1 #b1000)) (not $e16))) (not (and (not (and (not (and (not (= stato_1 #b0011)) (not $e30))) (not (and $e30 (not $e6))))) $e16)))) $e15)))) (not $e14)) #b1 #b0)))))))))) (bvnot (bvand (bvnot $e11) (bvnot (ite $e34 #b1 #b0)))))))))))) #b0))
(assert (! .f2 :interpolation-group .g0))
(define-fun .f3 () Bool (not (and (not $e29) $e10)))
(assert (! .f3 :interpolation-group .g0))
(define-fun .f4 () Bool (distinct (bvnot (bvand (bvnot (ite $e10 #b1 #b0)) (bvnot (bvand (bvnot (bvand reset_2 (bvnot (ite (= r_in_1 #b00000000) #b1 #b0)))) (bvnot (bvand (bvnot reset_2) (bvnot (bvand (bvnot (bvand $e11 (bvnot (ite (and (not (and (not (and (not (and (not $e29) (not $e14))) (not (and (not $e28) $e14)))) (not $e12))) (not (and (not $e28) $e12))) #b1 #b0)))) (bvnot (bvand (bvnot $e11) (bvnot (ite $e29 #b1 #b0)))))))))))) #b0))
(assert (! .f4 :interpolation-group .g0))
(define-fun .f5 () Bool (not (and (not $e26) $e10)))
(assert (! .f5 :interpolation-group .g0))
(define-fun .f6 () Bool (distinct (bvnot (bvand (bvnot (ite $e10 #b1 #b0)) (bvnot (bvand (bvnot (bvand reset_2 (bvnot (ite $e25 #b1 #b0)))) (bvnot (bvand (bvnot reset_2) (bvnot (bvand (bvnot (bvand $e11 (bvnot (ite (and (not (and (not (and (not (and (not (and (not (and (not $e26) (not $e15))) (not (and (not (and (not (and (not (and (not (and (not (= cont_1 (bvadd cont_2 #b00000001))) (not $e27))) (not (and $e27 (not $e25))))) (not $e16))) (not (and (not $e26) $e16)))) $e15)))) (not $e14))) (not (and (not $e26) $e14)))) (not $e12))) (not (and (not $e25) $e12))) #b1 #b0)))) (bvnot (bvand (bvnot $e11) (bvnot (ite $e26 #b1 #b0)))))))))))) #b0))
(assert (! .f6 :interpolation-group .g0))
(define-fun .f7 () Bool (not (and (not $e13) $e10)))
(assert (! .f7 :interpolation-group .g0))
(define-fun .f8 () Bool (distinct (bvnot (bvand (bvnot (ite $e10 #b1 #b0)) (bvnot (bvand (bvnot (bvand reset_2 (bvnot (ite (= cont1_1 #b00000000) #b1 #b0)))) (bvnot (bvand (bvnot reset_2) (bvnot (bvand (bvnot (bvand $e11 (bvnot (ite (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not (and (not $e23) (not $e13))) (not (and (not (and (not (and (not $e24) (not $e13))) (not (and $e24 (not $e22))))) $e23)))) (not $e20))) (not (and (not (and (not (and (not $e21) (not $e13))) (not (and (not $e22) $e21)))) $e20)))) (not $e19))) (not (and (not (= cont1_1 (bvadd cont1_2 r_in_2))) $e19)))) (not $e17))) (not (and (not (and (not (and (not (= cont1_1 cont_2)) (not $e18))) (not (and (not (= cont1_1 (bvmul cont_2 #b00000010))) $e18)))) $e17)))) (not $e15))) (not (and (not (and (not (and (not (= cont1_1 r_in_2)) (not $e16))) (not (and $e16 (not $e13))))) $e15)))) (not $e14))) (not (and $e14 (not $e13))))) (not $e12))) (not (and (not $e13) $e12))) #b1 #b0)))) (bvnot (bvand (bvnot $e11) (bvnot (ite $e13 #b1 #b0)))))))))))) #b0))
(assert (! .f8 :interpolation-group .g0))
(define-fun .f9 () Bool (not (and (not $e8) $e2)))
(assert (! .f9 :interpolation-group .g0))
(define-fun .f10 () Bool (distinct (bvnot (bvand (bvnot (ite $e2 #b1 #b0)) (bvnot (bvand (bvnot (bvand reset_1 (bvnot (ite $e3 #b1 #b0)))) (bvnot (bvand (bvnot reset_1) (bvnot (bvand (bvnot (bvand $e4 (bvnot (ite (and (not (and (not (and (not (and (not (and (not (and (not $e8) (not $e7))) (not (and $e7 (not $e3))))) (not $e6))) (not (and $e6 (not $e3))))) (not $e5))) (not (and $e5 (not $e3)))) #b1 #b0)))) (bvnot (bvand (bvnot $e4) (bvnot (ite $e8 #b1 #b0)))))))))))) #b0))
(assert (! .f10 :interpolation-group .g0))

(define-fun $ee1 () (_ BitVec 1) (bvand (bvnot clock_1) (bvnot reset_1)))
(define-fun $ee2 () (_ BitVec 1) (bvand (bvnot (bvand (bvnot reset_2) (bvnot reset_3))) (bvnot (bvand reset_2 reset_3))))
(define-fun $ee3 () (_ BitVec 1) (bvand $ee1 $ee2))
(define-fun $ee4 () Bool (= #b00000000 (bvadd x_out_0 (bvmul x_out_1 #b11111111))))
(define-fun $ee5 () (_ BitVec 1) (bvand reset_1 reset_2))
(define-fun $ee6 () (_ BitVec 1) (bvand (bvnot (bvand (bvnot reset_1) (bvnot reset_2))) (bvnot $ee5)))
(define-fun $ee7 () (_ BitVec 1) (bvand (bvnot (bvand (bvnot clock_1) (bvnot clock_2))) (bvnot (bvand clock_1 clock_2))))
(define-fun $ee8 () Bool (= x_out_0 #b00000000))
(define-fun $ee9 () (_ BitVec 1) (bvand (bvnot (bvand (bvnot clock_2) (bvnot clock_3))) (bvnot (bvand clock_2 clock_3))))
(define-fun $ee10 () (_ BitVec 1) (bvand $ee2 (bvnot $ee6)))
(define-fun $ee11 () (_ BitVec 1) (bvand (bvnot reset_1) $ee2))
(define-fun $ee12 () (_ BitVec 1) (bvand reset_2 $ee11))
(define-fun $ee13 () (_ BitVec 1) (bvand (bvnot reset_1) reset_2))
(define-fun $ee14 () (_ BitVec 1) (bvand (ite $ee4 #b1 #b0) (bvnot $ee6)))
(define-fun $ee15 () (_ BitVec 1) (bvand $ee9 (bvand (bvnot $ee7) $ee14)))
(define-fun $ee16 () (_ BitVec 1) (bvand $ee1 (ite $ee4 #b1 #b0)))
(define-fun $ee17 () (_ BitVec 1) (bvand reset_1 $ee6))
(define-fun $ee18 () (_ BitVec 1) (bvand $ee9 (bvand (ite $ee8 #b1 #b0) $ee17)))
(define-fun $ee19 () (_ BitVec 1) (bvand $ee6 (bvand reset_1 $ee2)))
(define-fun $ee20 () (_ BitVec 1) (bvand clock_1 (bvnot reset_1)))
(define-fun $ee21 () (_ BitVec 1) (bvand (ite $ee4 #b1 #b0) $ee20))
(define-fun $ee22 () (_ BitVec 1) (bvand (bvnot $ee7) (bvand (ite $ee4 #b1 #b0) $ee10)))
(define-fun $ee23 () (_ BitVec 1) (bvand clock_1 $ee11))


(define-fun .f11 () Bool (not (distinct (bvnot (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvnot (bvand (bvand (bvand reset_2 $ee3) (ite $ee4 #b1 #b0)) $ee6)) (bvnot (bvand (bvand (bvand $ee5 $ee7) (ite $ee8 #b1 #b0)) $ee9))) (bvnot (bvand $ee9 (bvand (bvnot $ee7) (bvand (ite $ee4 #b1 #b0) (bvand reset_2 (bvnot $ee6))))))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand reset_1 (bvand reset_2 $ee2)))))) (bvnot (bvand (bvnot $ee7) (bvand (ite $ee4 #b1 #b0) (bvand reset_2 $ee10))))) (bvnot (bvand (bvnot $ee7) (bvand $ee6 (bvand (ite $ee4 #b1 #b0) $ee12))))) (bvnot (bvand $ee9 (bvand (bvnot $ee7) (bvand $ee6 (bvand (ite $ee4 #b1 #b0) $ee13)))))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand $ee6 (bvand clock_1 $ee12)))))) (bvnot (bvand $ee9 (bvand $ee6 (bvand (ite $ee4 #b1 #b0) (bvand reset_2 $ee1)))))) (bvnot (bvand $ee9 (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand $ee6 (bvand clock_1 $ee13))))))) (bvnot (bvand (bvnot reset_2) (bvand clock_2 $ee15)))) (bvnot (bvand (bvnot reset_2) (bvand clock_2 (bvand $ee9 (bvand $ee7 $ee16)))))) (bvnot (bvand (bvnot reset_2) (bvand clock_2 $ee18)))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) $ee18)))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand (bvnot $ee9) (bvand (bvnot $ee2) $ee17))))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) (bvand (ite $ee8 #b1 #b0) $ee19))))) (bvnot (bvand (bvnot reset_2) (bvand (ite $ee8 #b1 #b0) (bvand (bvnot $ee9) $ee19))))) (bvnot (bvand (bvnot $ee9) (bvand (bvnot $ee7) (bvand (bvnot $ee2) $ee14))))) (bvnot (bvand (bvnot $ee9) (bvand (bvnot $ee2) (bvand $ee6 $ee16))))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand (bvnot $ee9) (bvand reset_1 (bvnot $ee2))))))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand (bvnot $ee9) (bvand (bvnot $ee2) (bvand $ee6 $ee20))))))) (bvnot (bvand $ee7 (bvand (bvnot $ee9) (bvand (bvnot $ee2) (bvand $ee6 $ee21)))))) (bvnot (bvand $ee7 (bvand (bvnot $ee9) (bvand (bvnot $ee2) $ee16))))) (bvnot (bvand (ite $ee8 #b1 #b0) (bvand $ee7 (bvand (bvnot $ee9) (bvand (bvnot $ee2) $ee20)))))) (bvnot (bvand $ee7 (bvand (bvnot $ee9) (bvand (bvnot $ee2) $ee21))))) (bvnot (bvand (bvnot $ee9) (bvand (bvnot $ee7) (bvand (bvnot $ee2) (bvand $ee6 (bvand (bvnot reset_1) (ite $ee4 #b1 #b0)))))))) (bvnot (bvand (bvnot reset_2) (bvand (bvnot $ee9) $ee22)))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) $ee22)))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) $ee15)))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) (bvand $ee7 (bvand (ite $ee4 #b1 #b0) $ee23)))))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) (bvand $ee9 (bvand $ee7 $ee21)))))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) (bvand (ite $ee8 #b1 #b0) (bvand $ee7 $ee23)))))) (bvnot (bvand (bvnot clock_2) (bvand (bvnot reset_2) (bvand $ee9 (bvand (ite $ee8 #b1 #b0) (bvand $ee7 $ee20))))))) (bvnot (bvand (bvnot reset_2) (bvand $ee7 (bvand (bvnot $ee9) (bvand $ee3 (ite $ee4 #b1 #b0)))))))) #b0)))

(assert (! .f11 :interpolation-group .g1))

(declare-fun y_out_1 () (_ BitVec 8))
(declare-fun y_out_0 () (_ BitVec 8))
(declare-fun f_1 () (_ BitVec 1))
(declare-fun f_2 () (_ BitVec 1))
(declare-fun f_3 () (_ BitVec 1))
(declare-fun f_4 () (_ BitVec 1))

(define-fun .f12 () Bool (= x_out_0 y_out_0))
(define-fun .f13 () Bool (= x_out_1 y_out_1))
(define-fun .f14 () Bool (not (= f_1 reset_1)))
(define-fun .f15 () Bool (not (= f_2 reset_2)))
(define-fun .f16 () Bool (not (= f_3 reset_3)))
(define-fun .f17 () Bool (not (= f_4 stbi_2)))

(assert (! .f12 :interpolation-group .g1))
(assert (! .f13 :interpolation-group .g1))
(assert (! .f14 :interpolation-group .g1))
(assert (! .f15 :interpolation-group .g1))
(assert (! .f16 :interpolation-group .g1))
(assert (! .f17 :interpolation-group .g1))

(check-sat)
(get-interpolant (.g0))
