# EJERCICIOS UNIX 2

## Ejercicio 1
Sitúe como directorio actual su directorio `HOME` y **no** se mueva de él durante **todo** el ejercicio. Los siguientes **comandos** se van a **ejecutar** sobre el fichero `/etc/passwd`

**a)** Liste las columnas (campos) `1` y `3` del **fichero** teniendo en cuenta que tiene varias columnas que están separadas entre ellas mediante el carácter `:`.

```bash
$ cut -d ":" -f 1,3 /etc/passwd
```
> La opción `-d` indica el delimitador y `-f` los campos a mostrar.

**b)** Liste las columnas **comprendidas** entre los caracteres `2` a `7`.

```bash
$ cut -c 2-7 /etc/passwd
```
> La opción `-c` indica los caracteres a mostrar, en este caso del 2 al 7 ambos incluidos.

**c)** Muestre las líneas que contienen la cadena de caracteres `bash`.

```bash
$ grep "bash" /etc/passwd
```
> El comando `grep` busca la cadena de caracteres `bash` en el fichero `/etc/passwd`.

**d)** Muestre las líneas del fichero que contienen el número exacto `15`, es decir, que contengan esa cadena literalmente y no dentro de otra. Por ejemplo, no debe mostrar líneas que contengan la cantidad `40615` o `19155`.

```bash
$ grep -w "15" /etc/passwd # FORMA ADECUADA
# Otra forma usando regexp
$ grep "\<15\>" /etc/passwd
# Otra forma, en este caso solo salva los números 15 que no estén rodeados de otros números
$ grep [^0-9]15[^0-9] /etc/passwd
```
> La opción `-w` busca la cadena de caracteres `15` como palabra completa.

**e)** Cuente el número de líneas, sólo el número de líneas del fichero.

```bash
$ wc -l /etc/passwd
# Otra forma
$ grep -c "" /etc/passwd
```
> La opción `-l` de `wc` cuenta el número de líneas.
>
> La opción `-c` de `grep` cuenta el número de líneas que contienen la cadena de caracteres.

**f)** **Copie** el fichero a tu directorio `HOME`. **Sustituya** todas las letras **minúsculas** por **mayúsculas**, mostrando el resultado por la salida estándar.

```bash
$ cp /etc/passwd ~/passwd
$ tr [:lower:]' '[:upper:]' < ~/passwd
```
> El comando `tr` usando RegExp permite `[:lower:]` para interpretar un patrón de letras minusculas y lo traduce a `[:upper:]`, un patrón que traduce cada letra a su mayuscula.

**g)** Muestre las 20 primeras líneas del fichero.

```bash
$ head -n 20 etc/passwd
```
> La opción `-n` de `head` indica el número de líneas a mostrar.

**h)** **Concatene** las líneas del fichero del enunciado con la **copia** realizada a su `HOME`, usando como carácter separador el `;`, es decir, se han de **mostrar** por **pantalla** cada **línea** repetida **dos** veces **separadas** por `;`.

```bash
$ paste -d ";" /etc/passwd ~/passwd
```
> La opción `-d` de `paste` indica el delimitador entre las dos partes concatenadas.

**i)** Ordene en orden inverso.

```bash
$ tac /etc/passwd # TODO: PREGUNTAR SI SE PUEDE USAR TAC
```
> El comando `tac` muestra el contenido de un fichero en orden inverso (es `cat` escrito al revés).


## Ejercicio 2
a) **Indique** cuál o cuáles de las siguientes **líneas** de **comandos** son **equivalentes** entre sí, considerando **equivalentes** a aquellas cuyas **informaciones** de **entrada** y **salida** sean las mismas:

**1)**
```bash
$ cat [ruta]fichero | grep cadena caracteres | wc -l
```
**2)**
```bash
$ cat | grep cadena caracteres | wc -l
```
**3)**
```bash
$ cat < [ruta]fichero | grep cadena caracteres | wc -l
```

> Las líneas 1 y 3 son equivalentes, ya que ambas leen el contenido de un fichero y lo pasan a `grep` para que busque una cadena de caracteres y finalmente `wc -l` cuenta el número de líneas que contienen la cadena de caracteres.

**b)** Sobre las **anteriores** líneas de **comandos**, proponga una **equivalente** a la **primera** pero con tan solo **dos** comandos en la **tubería**, o sea, `comando 1 | comando 2`.

```bash
$ grep cadena_caracteres /etc/passwd | wc -l
```
> En este caso, `grep` busca la cadena de caracteres en el fichero `/etc/passwd` y `wc -l` cuenta el número de líneas que contienen la cadena de caracteres, lo mismo que hace el `1)`.


## Ejercicio 3
**Algunas** de las siguientes líneas de comando son **erróneas**. Puede **no** dar **error** de **ejecución** pero ser **semánticamente incorrecta**. Indique para **cada una**, si es **correcta** o **no** (compruebe las condiciones necesarias tanto para redirección como para tuberías) y, en caso de error, el error cometido corrigiendo aquellas donde sea psoble:

**a)**
```bash 
$ cat | grep cadena_caracteres [ruta]fichero | wc -l
```
> **Incorrecta**. El comando `cat` no tiene sentido sin un fichero de entrada. La forma correcta sería:
```bash
$ cat [ruta]fichero | grep cadena_caracteres | wc -l
```

**b)**
```bash
$ sort | uniq < [ruta]fichero > [ruta]fichero
```
> **Incorrecta**. El comando `sort` no recibe un fichero de entrada, lo que no tiene sentido. Además, el comando uniq intenta leer del mismo sitio donde escribe, lo cual no va a resultar con lo que se quiere. La forma correcta sería:
```bash
$ sort [ruta]fichero | uniq > [ruta]fichero
```
> De esta forma, `sort` ordena el contenido del fichero y `uniq` elimina las líneas duplicadas, guardando el resultado en el mismo fichero.

**c)** 
```bash
$ cp [ruta]fichero/s_origen > [ruta]fichero/s_destino
```
> **Incorrecta**. El comando `cp` no recibe un fichero de salida ya que no escribe nada por la salida estandar. La forma correcta sería:
```bash
$ cp [ruta]fichero/s_origen [ruta]fichero/s_destino
```

**d)**
```bash
$ cat [ruta]fichero/s > [ruta]fichero | grep cadena caracteres
```
> **Incorrecta**. La redirección de salida `>` se está usando en la tubería, lo cual no tiene sentido ya que al grep no le llegará ninguna entrada. La forma correcta sería:
```bash
$ cat [ruta]fichero/s | grep cadena caracteres > [ruta]fichero
```

**e)** 
```bash
$ ll -d /usr/p* > grep ^d pa
```
> **Incorrecta**. El comando `grep` no recibe un fichero de entrada y no tiene sentido usar un comando como salida. La forma correcta sería:
```bash
$ ll -d /usr/p* | grep ^d > pa
```

**f)**
```bash
$ cat fichero1 fichero2 | cmp
```
> **Incorrecta**. El comando `cmp` necesita dos ficheros de entrada para comparar, por lo que la forma correcta sería:
```bash
$ cat fichero1 fichero2 | cmp fichero_extra
```


## Ejercicio 4
El **siguiente** comando `UNIX` busca **nombres de ficheros** y **directorios** a partir de un **determinado directorio** (en el caso del comando indicado el directorio raíz):

```bash
$ find / -name passwd -print
```

Como se puede ver al ejecutarlo, se mezclan la salida estándar y la salida de error. El comando tarda bastante en finalizar por lo que se puede abortar su ejecución pulsando `CTRL + C`. Mediante el procedimiento adecuado, haga que la salida estándar se almacene en el fichero `resultado_busqueda` y los mensajes de error que aparecen se añadan a los ya existentes en un fichero llamado `errores`. Ubicar ambos ficheros en su `HOME`.

```bash
$ find / -name passwd -print > ~/resultado_busqueda 2>> ~/errores
```
> La redirección `>` guarda la salida estándar en el fichero `resultado_busqueda` y `2>>` añade los mensajes de error al fichero `errores`, concatenando con lo que ya haya dentro de el mismo.


## Ejercicio 5
**a)** Muestre por pantalla el número de líneas del **fichero** `/etc/passwd` que contienen la cadena `false`.

```bash
$ grep "false" /etc/passwd | wc -l
# Otra forma
$ grep -c "false" /etc/passwd
```
> La opción `-c` de `grep` cuenta el número de líneas que contienen la cadena de caracteres.

**b)** Mediante **tuberías**, cree una **línea de comandos** cuyo resultado sea el **número de ficheros** y **directorios** de un determinado directorio.

```bash
$ ls directorio | wc -l
```
> El comando `ls` lista los ficheros y directorios de un directorio y `wc -l` cuenta el número de líneas que devuelve `ls`.

**c)** **Afinando** un poco más, cree dos **líneas de comandos diferentes**, de forma que una dé como **resultado** el número de ficheros, **SOLO FICHEROS**, de un determinado directorio, y la otra dé como resultado el número de directorios, **SOLO DIRECTORIOS**, de un determinado directorio.

```bash
$ ls -l directorio | grep -c "^-" # SOLO FICHEROS
$ ls -l directorio | grep -c "^d" # SOLO DIRECTORIOS
```
> La opción `-l` de `ls` muestra información detallada de los ficheros y directorios, y `grep` filtra las líneas que empiezan por `-` o `d` para contar el número de ficheros o directorios, respectivamente.


## Ejercicio 6
Copie a su directorio `HOME` el fichero `/home/jiramos/tmp/fich_ej6.txt`.

**a)** **Cree** un **nuevo fichero** que contenga las líneas que **no** tengan caracteres numéricos **(0, 1, 2, ..., 9)**.

```bash
$ grep -v "[0-9]" ~/fich_ej6.txt > ~/fich_ej6_SIN_NUMEROS.txt
```
> La opción `-v` de `grep` invierte el patrón de busqueda, en este caso busca las lineas que **NO** contengan números.

**b)** Busque las apariciones de la cadena de caracteres `linea` en el fichero, y que todas las líneas que la contengan sean ordenadas de forma alfabética en orden inverso, eliminando todas las posibles líneas repetidas.

```bash
$ grep "linea" ~/fich_ej6.txt | sort -r | uniq
```
> El comando `grep` busca la cadena de caracteres `linea` en el fichero, `sort -r` ordena las líneas de forma alfabética en orden inverso y `uniq` elimina las líneas repetidas contiguas.