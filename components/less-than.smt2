(declare-const rhole_left (_ BitVec 32))
(declare-const rhole_right (_ BitVec 32))
(declare-const rreturn Bool)
(assert (= rreturn (bvslt rhole_left rhole_right)))
