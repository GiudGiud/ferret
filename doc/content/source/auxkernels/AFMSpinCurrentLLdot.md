# AFMSpinCurrentLLdot

!syntax description /AuxKernels/AFMSpinCurrentLLdot

## Overview

Computes the $k^\mathrm{th}$ component of the antiferromagnetic (AFM) spin current contribution due to

\begin{equation}
  \begin{aligned}
    j_k^\mathrm{L\dot{L}} = f \epsilon_{ijk} L_i \frac{\partial L_j}{\partial x_j}
  \end{aligned}
\end{equation}

where $\epsilon_{ijk}$ is the Levi-Civita pseudo-tensor and $f$ is a factor that can be used to scale the postprocessed quantity. Note that $\mathbf{L}$ and $\mathbf{m}$ are normalized quantities from the FERRET output. The total spin current is $$

\begin{equation}
  \begin{aligned}
    j_k^\mathrm{total} = j_k^\mathrm{L\dot{L}}+j_k^\mathrm{L\dot{m}}+j_k^\mathrm{m\dot{L}}+j_k^\mathrm{m\dot{m}}
  \end{aligned}
\end{equation}

as described in [!cite](Cheng2014). The vectors $\mathbf{L}$ and $\mathbf{m}$ are the antiferromagnetic Néel and total magnetization vectors respectively.

## Example Input File Syntax

!! Describe and include an example of how to use the AFMSpinCurrentLLdot object.

!syntax parameters /AuxKernels/AFMSpinCurrentLLdot

!syntax inputs /AuxKernels/AFMSpinCurrentLLdot

!syntax children /AuxKernels/AFMSpinCurrentLLdot
