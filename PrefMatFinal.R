# Data matrix, points matrix, and preference matrix (input data sets for Gale's TTC algorithm implementation).
# Miren Lur Barquin Torre. 19 February 2020.

#install.packages("truncnorm")
library(truncnorm)
#install.packages("MASS")
library(MASS)

#######################################################################
##########       DATA MATRIX       ####################################
#######################################################################
proctime<-proc.time()
#zero matrix (nCouples x 7)
nCouples<-10
dataMat<-matrix(data=0,nCouples,7,byrow=TRUE)
dataMat<-as.data.frame(dataMat)


# C1: Patients' blood group.

probABOPatient <- c(0.559,0.284,0.13,0.027)
auxABOp<-rmultinom(1, nCouples , probABOPatient )

dataMat[,1]=rep(c("O","A","B","AB"),auxABOp)


# C2: Donors' blood group.

o=0.297; a=0.499; b=0.158; ab=0.046 #Prob. ABO group donors.

p<-0.507    # The 50.7% of initial patient-donor pairs have ABO incopatibilities.
for(i in 1:nCouples){
  t<-runif(1,min=0, max=1)
  compat<-runif(1,min=0, max=1)
  if((compat <= p) & (dataMat[i,1]!="AB")){ # if incompatible (group AB has no ABO incompatibilities)
    if(dataMat[i,1]=="O"){        # O group patient
      if(t<=(a/(a+b+ab))){
        dataMat[i,2]="A"}
      else if(t<=((a+b)/(a+b+ab))){
        dataMat[i,2]="B"}
      else{dataMat[i,2]="AB"}}
    
    else if(dataMat[i,1]=="A"){   # A group patient
      if(t<=(b/(b+ab))){
        dataMat[i,2]="B"}
      else{dataMat[i,2]="AB"}}
    
    else if(dataMat[i,1]=="B"){   # B group patient 
      if(t<=(a/(a+ab))){
        dataMat[i,2]="A"}
      else{dataMat[i,2]="AB"}}
    
  }else{  # if compatible
    if(dataMat[i,1]=="O"){        # O group patient
      dataMat[i,2]="O"}
    
    else if(dataMat[i,1]=="A"){   # A group patient
      if(t<=(o/(o+a))){
        dataMat[i,2]="O"}
      else{
        dataMat[i,2]="A"}}
    
    else if(dataMat[i,1]=="B"){   # B group patient
      if(t<=(o/(o+b))){
        dataMat[i,2]="O"}
      else{dataMat[i,2]="B"}}
    
    else{                        # AB group patient
      if(t<=o){
        dataMat[i,2]="O"}
      else if(t<=(o+a)){
        dataMat[i,2]="A"}
      else if(t<=(o+a+b)){
        dataMat[i,2]="B"}
      else{dataMat[i,2]="AB"}}}}


# C3: 'matching' probability.

for(i in 1:nCouples){
  t=runif(1,min=0, max=1)
  if(t<=0.4645){
    dataMat[i,3]=100-0}
  else if(t<=(0.142+0.4645)){
    dataMat[i,3]=100-(runif(1,min=1, max=49.99))}
  else if(t<=(0.1006 +0.142+0.4645)){
    dataMat[i,3]=100-(runif(1,min=50, max=79.99))}
  else{
    dataMat[i,3]=100-(runif(1,min=80, max=100.99))}}


# C4: number of months in dialisis.

auxCol4<-replicate(nCouples,0)
auxMonths<-rtruncnorm(round(nCouples*0.782), a=0, b=297, mean=55.7, sd=61.7) 

auxCol4[1:round(nCouples*0.782)]=round(auxMonths)  # 21.8% of the patient have not started dialysis yet. 

auxCol4=sample(auxCol4)
dataMat[,4]=auxCol4


# C5: Patients' ages. 

auxAgeP<-rtruncnorm(nCouples, a=7, b=72, mean=47.2, sd=11.9)
dataMat[,5]=round(auxAgeP)


# C6: Agents' ages. 

auxAgeD<-rtruncnorm(nCouples, a=19, b=74, mean=49.6, sd=10.6) 
dataMat[,6]=round(auxAgeD)


# C7: Autonomous Communities.
# A.C.           Notation     % 
# Andalucia         1      31.7
# Aragon            2       1.2
# Canary Islands    3       1.5
# Cantabria         4       0.9
# Catalonia         5      45
# Valencian C.      6       1.5 
# Galicia           7       5.6
# Madrid            8       5
# Basque Country    9       7.7

probAutComm<-c(0.317,0.012,0.015,0.009,0.45,0.015,0.056,0.05,0.077)
auxAutComm<-rmultinom(1, nCouples , probAutComm )
auxCol7<-replicate(nCouples,0)
 
auxCol7=rep(c(1,2,3,4,5,6,7,8,9),auxAutComm)

auxCol7=sample(auxCol7)

dataMat[,7]=auxCol7

##############################################################
#############      POINTS MATRIX        ######################
##############################################################

# Zero matrix (nCouples x nCouples). Rows->donors & columns->patients.

pointsMat<-matrix(data=0,nCouples,nCouples,byrow=TRUE)

# First: all the patient-donor pairs that enter the program are incompatible, therefore we do not need to add points to position (i,i) for all i=1,..., nCouples but also is known that for any patient is better to stay with its initial object than receive another incompatible one. To represent this and make easier to order the preferences, let's give elements (i,i) in the diagonal of the matrix value -1 and incompatibilities -2 (s.t. they stay under the initial object in the preferences).

for(i in 1:nCouples){
  pointsMat[i,i]=-1
}

for(i in 1:nCouples){
  for(j in 1:nCouples){
    if(i!=j){ # non isogroup
      if((dataMat[i,1]=="O" & dataMat[j,2]!="O")|(dataMat[i,1]=="A" &(dataMat[j,2]=="B"|dataMat[j,2]=="AB"))|(dataMat[i,1]=="B" & (dataMat[j,2]=="A"|dataMat[j,2]=="AB"))){
        pointsMat[j,i]=-2
      }else{
        # ISOGROUP
        if(dataMat[i,1]==dataMat[j,2])
          {pointsMat[j,i]=pointsMat[j,i]+30}
        # MATCHING PROBABILITY
        if(dataMat[j,3]<=25){pointsMat[j,i]=pointsMat[j,i]+30}
        else if(dataMat[j,3]<=50){pointsMat[j,i]=pointsMat[j,i]+20}
        else if(dataMat[j,3]<=75){pointsMat[j,i]=pointsMat[j,i]+10}
        # AGE DIFFERENCE
        # In the case of sensitized recipient (PRA>50), ABO group O recipients, the donor belongs to ABO group AB or if the recipient is a child, they will accept the age difference > 10 and they will receive 30 points.
        if( dataMat[j,5]<16|dataMat[j,1]==1|dataMat[j,2]==4|dataMat[j,3]<=50){pointsMat[j,i]=pointsMat[j,i]+30}
        else if(abs(dataMat[i,5]-dataMat[j,6])<=10 & abs(dataMat[j,5]-dataMat[i,6])<=10){pointsMat[j,i]=pointsMat[j,i]+30}
        else if (abs(dataMat[i,5]-dataMat[j,6])<=10 | abs(dataMat[j,5]-dataMat[i,6])<=10){pointsMat[j,i]=pointsMat[j,i]+15}
        # MONTHS ON DIALYSIS
        pointsMat[j,i]=pointsMat[j,i]+0.05*dataMat[j,4]
        # Child recipients (<16 years) will receive +30 points (already done in AGE section) and the donor must be <=50 years old.
        if(dataMat[i,5]<16&dataMat[j,6]>50){pointsMat[j,i]=-2}
        # The Autonomous Community will be used later just in the case of tie.
      }}}}

# in the case that there are ties in the columns, if both pairs belong to the same Autonomous community add 5 points.
for(i in 1:nCouples){
  repetitions<-c()
  duplicates=pointsMat[,1][duplicated(pointsMat[,1])]
  for(k in duplicates){
    if(is.element(k, repetitions)==FALSE & k!=-2){
      repetitions<-append(repetitions,k)
      for(j in 1:nCouples){
        if(pointsMat[j,i]==k){ 
          if(dataMat[i,7]==dataMat[j,7]){
            pointsMat[j,i]=pointsMat[j,i]+5
          }}}}}}



######################################################################################################################
##########################     PREFERENCE MATRIX   ###################################################################
######################################################################################################################

prefMat<-matrix(data=0,nCouples,nCouples,byrow=TRUE)

for(i in 1:nCouples){
  auxMat<-matrix(data=0,nCouples,2,byrow=TRUE)
  for(j in 1:nCouples){
    auxMat[j,1]=j
    auxMat[j,2]=pointsMat[j,i]
  }
  auxDataFr<-as.data.frame(auxMat)
  auxDataFr<-auxDataFr[order(auxDataFr$"V2", decreasing = TRUE),]
  
  #if there are still ties in some column we order them randomly. This will not be applied for objects with -2 points, since they will not take part in the problem because of incompatibilities, and therefore, it is not necessary.
  duplicates<-auxDataFr[duplicated(auxDataFr$"V2"),]
  auxVect<-c()
  for(p in duplicates[,"V2"]){
    if(is.element(p,auxVect)==FALSE & p!=-2){
      auxVect<-append(auxVect,p)
    }
  }
  if(length(auxVect)!=0){
    for(q in auxVect){
      auxInd<-c()
      for(l in 1:nCouples){
        if(auxDataFr[l,"V2"]==q){
          auxInd<-append(auxInd,auxDataFr[l,"V1"])
        }}
      auxIndRand=sample(auxInd)
      pos<-0
      for(m in 1:length(auxIndRand)){
        for(n in pos+1:nCouples){
          pos=n
          if(auxDataFr[n,"V2"]==q){
            auxDataFr[n,"V1"]=auxIndRand[m]
            break
          }}}}}
  
  for(k in 1:nCouples){
    prefMat[k,i]=auxDataFr[k,"V1"]
  }
}

matrx<-prefMat

write.matrix(matrx,file="RPrefMat.dat", sep=" ")
dataMat
pointsMat
prefMat
proc.time()-proctime