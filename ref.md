# 参考

1. [SVPWM](https://zhuanlan.zhihu.com/p/47766452)
2. [FOC](https://zhuanlan.zhihu.com/p/147659820 "彭志辉")

## PARK变换

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

## Clark变换

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

## [伏秒原则](https://www.zhihu.com/question/437481153/answer/1667097183)

伏秒原则，处于稳定状态的电感，开关导通时间（电流上升段）的伏秒数须与开关关断（电流下降段）时的伏秒数在数值上相等，尽管两者符号相反。这也表示，绘出电感电压对时间的曲线，导通时段曲线的面积必须等于关断时段曲线的面积

伏秒平衡原理：在稳态工作的开关电源中电感两端的正伏秒值等于负伏秒值。伏秒数也称为伏秒积，即电感两端的电压V和开关动作时间T二者的乘积。伏秒原则，又称伏秒平衡，是指开关电源稳定工作状态下，加在电感两端的电压乘以导通时间等于关断时刻电感两端电压乘以关断时间，或指在稳态工作的开关电源中电感两端的正伏秒值等于负伏秒值。在一个周期 T 内， 电感电压对时间的积分为 0，称为伏秒平衡原理。任何稳定拓扑中的电感都是传递能量而不消耗能量， 都会满足伏秒平衡原理 。

当开关电源电路处于稳态工作时，一个开关周期内电感的电流变化量最终为零，即开关导通时通过电感的电流增加量和开关断开时电感的电流减少量是相等的。换句话说，处于稳定工作状态的开关电路中，一个周期因开关作用被分为两段，其中开关导通时间内电感电流在增加，开关关断时间内电感电流在减少，那么在一个周期内，电流的增加量与电流的减少量是相等的，即：$\Delta I_{on}=\Delta I_{off}$。伏秒原则可以保证电感不会出现偏磁现象，不会出现饱和。分析开关电源中电容和电感的几条原则：电容两端的电压不能突变 (当电容足够大时,可认为其电压不变)；电感中的电流不能突变 (当电感足够大时,可认为其电流恒定不变)；流经电容的电流平均值在一个开关周期内为零；电感两端的伏秒积在一个开关周期内必须平衡。
