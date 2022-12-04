1. [SVPWM](https://zhuanlan.zhihu.com/p/47766452)
2. [FOC](https://zhuanlan.zhihu.com/p/147659820 "彭志辉")

# PARK变换

变换

$$
\mathbf{i}_{dq0} = \mathbf{P}\mathbf{i}_{abc}=
\frac23\begin{bmatrix} 
cos\theta	&cos(\theta-120\degree)	&cos(\theta+120\degree)\\
-sin\theta	&-sin(\theta-120\degree)	&-sin(\theta+120\degree)\\ 
\frac12& \frac12& \frac12
\end{bmatrix}
\begin{bmatrix}
i_a\\i_b\\i_c
\end{bmatrix}
$$

反变换

$$
\mathbf{i}_{abc} = \mathbf{P}^{-1}\mathbf{i}_{abc}=
\begin{bmatrix} 
cos\theta 	&-sin\theta	& 1\\
cos(\theta-120\degree)&-sin(\theta-120\degree)&1\\ 
cos(\theta+120\degree)&-sin(\theta+120\degree)& 1
\end{bmatrix}
\begin{bmatrix}
i_d\\i_q\\i_0
\end{bmatrix}
$$

# Clark变换

$$
\begin{bmatrix}
\mathbf{I_\alpha}  \\
 \mathbf{I_\beta}
 \end{bmatrix}=

\begin{bmatrix}
1&-\frac12&-\frac12\\
0&\frac{\sqrt3}2&-\frac{\sqrt3}2
\end{bmatrix}

\begin{bmatrix}
\mathbf{I_a}\\
\mathbf{I_b}\\
\mathbf{I_c}\\
\end{bmatrix}
$$
