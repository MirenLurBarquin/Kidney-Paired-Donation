# Kidney-Paired-Donation
This repository contains the code for my Mathematics BSc thesis on ***The assignment problem for Kidney Paired Donation***, conducted at the University of the Basque Country (UPV/EHU) between September 2019 and March 2020.

The project involves the implementation of Gale's Top Trading Cycles (TTC) algorithm for kidney paired donation. The objective is to optimize the pairing of patients and donors from *n* incompatible pairs based on the selection and prioritization criteria established by the Spanish National Transplant Organization (ONT) in 2015 for the Spanish paired renal donation program [1]. Due to time constraints, limited information, or challenges in data acquisition, certain simplifications were made when applying the criteria.

The full text is available at: [Link](https://addi.ehu.es/handle/10810/49108)


- **PrefMatFinal.R:** generates both the data matrix and its corresponding preference matrix.
- **TTC_ImplementationCode.cpp:** implementation of Gale's TTC algorithm, which takes as input the matrices generated in PrefMatFinal.R.


## References
1. Organización Nacional de Trasplantes. (2015). Ministerio de Sanidad, Consumo y Bienestar Social. Programa Nacional de Donaci ́on Cruzada en España. Retrieved January 19, 2020, from [Link](http://www.ont.es/infesp/DocumentosDeConsenso/Programa%20Donaci%C3%B3n%20Renal%20Cruzadaactualizaci%C3%B3n25062015.pdf).
