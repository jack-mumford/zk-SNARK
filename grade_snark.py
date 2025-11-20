#!/usr/bin/env python3
import json, random, subprocess, sys
from pathlib import Path

P = 2_147_483_647  # 2^31-1
DOM = [1,2,3]      # constraint indices

# ----- finite field ops -----
def f_add(x,y): return (x+y) % P
def f_sub(x,y): return (x-y) % P
def f_mul(x,y): return (x*y) % P
def f_pow(b,e):
    res=1;b%=P
    while e:
        if e&1: res=f_mul(res,b)
        b=f_mul(b,b); e>>=1
    return res
def f_inv(x): return f_pow(x, P-2)

# Lagrange basis value L_i(tau) over domain {1,2,3}
def lagrange_eval(x_point, i):
    num, den = 1, 1
    for j in DOM:
        if j == i: continue
        num = f_mul(num, f_sub(x_point, j))
        den = f_mul(den, f_sub(i, j))
    return f_mul(num, f_inv(den))

# Build CRS evals at tau from fixed R1CS (rows 1..3)
def build_crs_evals(tau):
    # Variable order z=(1,a,b,w,c,s,y) indices 0..6
    # 1) a*b=c
    L1 = [0,1,0,0,0,0,0]; R1 = [0,0,1,0,0,0,0]; O1 = [0,0,0,0,1,0,0]
    # 2) (a+b)*1=s
    L2 = [0,1,1,0,0,0,0]; R2 = [1,0,0,0,0,0,0]; O2 = [0,0,0,0,0,1,0]
    # 3) w*(c - s) = y - s
    L3 = [0,0,0,1,0,0,0]
    R3 = [0,0,0,0,1,(P-1)%P,0]
    O3 = [0,0,0,0,0,(P-1)%P,1]

    Lrows = [L1,L2,L3]; Rrows=[R1,R2,R3]; Orows=[O1,O2,O3]

    Li_tau = [lagrange_eval(tau, i) for i in DOM]  # i=1..3
    A_eval=[0]*7; B_eval=[0]*7; C_eval=[0]*7

    for j in range(7):
        sA = sB = sC = 0
        for i in range(3):
            sA = f_add(sA, f_mul(Lrows[i][j], Li_tau[i]))
            sB = f_add(sB, f_mul(Rrows[i][j], Li_tau[i]))
            sC = f_add(sC, f_mul(Orows[i][j], Li_tau[i]))
        A_eval[j], B_eval[j], C_eval[j] = sA, sB, sC

    # t(X) = ∏_{i=1}^3 (X - i)
    t_eval = 1
    for i in DOM:
        t_eval = f_mul(t_eval, f_sub(tau, i))
    return A_eval, B_eval, C_eval, t_eval

def run_student(exe, payload):
    proc = subprocess.run([exe], input=payload.encode(),
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=2)
    if proc.returncode != 0:
        raise RuntimeError(f"Student program exit {proc.returncode}: {proc.stderr.decode()}")
    return json.loads(proc.stdout.decode().strip())

def score_case(out, a,b,w, A_eval,B_eval,C_eval,t_eval):
    # Recompute y
    c = f_mul(a,b)
    s = f_add(a,b)
    y = f_add(s, f_mul(w, f_sub(c,s)))
    ok_y = (out["y"] % P) == y

    # Recompute A_tau,B_tau,C_tau from witness and CRS
    z = [1,a,b,w,c,s,y]
    def lin(evals):
        acc=0
        for ev, zj in zip(evals,z):
            acc = f_add(acc, f_mul(ev, zj))
        return acc
    A_tau = lin(A_eval); B_tau = lin(B_eval); C_tau = lin(C_eval)
    ok_lin = (out["A_tau"]%P)==A_tau and (out["B_tau"]%P)==B_tau and (out["C_tau"]%P)==C_tau

    # Identity
    lhs = f_sub(f_mul(out["A_tau"]%P, out["B_tau"]%P), out["C_tau"]%P)
    rhs = f_mul(out["H_tau"]%P, t_eval)
    ok_eq = (lhs == rhs)
    return ok_y, ok_lin, ok_eq

def main():
    # Build with warnings captured for cleanliness score
    subprocess.run(["make", "clean"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    build = subprocess.run(["make"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    build_out = build.stdout.decode()
    if build.returncode != 0:
        print("Build failed:\n", build_out)
        print("\n=== Score: 0/100 (build failed) ===")
        sys.exit(1)
    exe = "./student_snark"
    if not Path(exe).exists():
        print("student_snark not found after build.")
        print("\n=== Score: 0/100 ===")
        sys.exit(1)

    # Style points: no 'warning' => 10 pts, otherwise 5 pts
    style_pts = 10 if "warning" not in build_out.lower() else 5

    random.seed(42)
    tau = random.randrange(1, P)  # secret toxic waste
    A_eval,B_eval,C_eval,t_eval = build_crs_evals(tau)

    tests=[]
    for _ in range(10):
        a = random.randrange(0, P)
        b = random.randrange(0, P)
        w = random.randrange(0, 2)
        tests.append((a,b,w))

    y_pts = lin_pts = eq_pts = 0
    for idx,(a,b,w) in enumerate(tests,1):
        payload = json.dumps({
            "p": P,
            "crs": {
                "A_eval": A_eval, "B_eval": B_eval, "C_eval": C_eval, "t_eval": t_eval
            },
            "instance": { "a": a, "b": b, "w": w }
        })
        try:
            out = run_student(exe, payload)
            ok_y, ok_lin, ok_eq = score_case(out, a,b,w, A_eval,B_eval,C_eval,t_eval)
        except Exception as e:
            print(f"Case {idx}: runtime error: {e}")
            ok_y = ok_lin = ok_eq = False

        # Per case: y=3 pts, lin=3 pts, eq=3 pts -> 9 per case, 90 total for 10 cases
        y_pts  += 3 if ok_y  else 0
        lin_pts+= 3 if ok_lin else 0
        eq_pts += 3 if ok_eq else 0
        print(f"Case {idx}: y:{ok_y} lin:{ok_lin} eq:{ok_eq}  (+{(3 if ok_y else 0)+(3 if ok_lin else 0)+(3 if ok_eq else 0)})")

    total = y_pts + lin_pts + eq_pts + style_pts
    print("\n=== Category breakdown (out of 100) ===")
    print(f"Correct public output y:           {y_pts:>3}/30")
    print(f"Correct linear combos A,B,C:       {lin_pts:>3}/30")
    print(f"QAP identity (A*B-C == H*t):       {eq_pts:>3}/30")
    print(f"Code cleanliness (-Wall -Wextra):  {style_pts:>3}/10")
    print(f"\n=== Total: {total}/100 ===")

if __name__ == "__main__":
    main()
