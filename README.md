# INF3105 TP2

### Requis : 
  g++ => 4.8.5  
  GNU Make 3.82  
  
  
### Optionnel : 
  [cmake](https://cmake.org)  
  
  
### Pour compiler :
  ```
  make
  ```
  
  
### Execution : 
  ```
  ./tp2 <FILE_NAME>
  ```
  
  
***

## Notes :
> FILE_NAME est une base de connaissance qui contient des foncteurs et des types.

1. _type_ s = {s1, ..., Si}
..1. _si_ est unique et _si_ représente les valeurs possibles pour _s_  
  _s_ est unique
  
2. _foncteur_ f :: v0, ..., vi  
  (s[0][0], ..., s[0][i])  
  ...  
  (s[j][0], ..., s[j][i])  
  * un foncteur a une définition et des clauses. il est défini par i types et contient j+1 clauses.  
  * chaque clause contient i valeurs où s[j][i] est une valeur possible du type v1 vi
