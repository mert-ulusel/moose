# CoupledConvectiveHeatFluxBC

!syntax description /BCs/CoupledConvectiveHeatFluxBC

This boundary condition computes convective heat flux $q'' = Hw \cdot (T - T_{inf})$, where $Hw$ is convective heat transfer coefficient,
$T$ is the temperature solved for, and $T_{inf}$ is far field temperature.  Both $Hw$ and $T_{inf}$ are spatially varying variables.

A typical use case for this boundary condition are coupled multi-apps exchanging heat flux.

It is possible to use vector coupling to compute the heat flux for multi-phase fluids. In this case,
users need to supply `alpha` parameter, which represents the volume fraction for each phase. Similarly,
multiple components have to be supplied for `htc` and `T_infinity`. The number of components for `alpha`, `Hw` and `T_infinity` must match.
The heat flux is then computed as $q'' = \Sigma_k \alpha^k \cdot Hw^k \cdot (T - T_{inf}^k)$.

!listing coupled_convective_heat_flux_two_phase.i block=BCs/right


!syntax parameters /BCs/CoupledConvectiveHeatFluxBC

!syntax inputs /BCs/CoupledConvectiveHeatFluxBC

!syntax children /BCs/CoupledConvectiveHeatFluxBC
