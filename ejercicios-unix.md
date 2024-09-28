# EJERCICIOS UNIX

## Ejercicio 1
Mediante el uso de los comandos cd, ls/ll y pwd, dibuje de forma gráfica similar a como aparece en los apuntes, los siguientes elementos de la estructura de directorios de la máquina:

**a)** Cinco directorios hijos del directorio raíz distintos a los de los apuntes.

```bash
$ ls /
boot run usr media proc
```

**b)** Los directorios que aparecen en la **ruta absoluta** su directorio HOME. ¿Cuál es el directorio padre de su directorio HOME?

```bash
$ cd
$ pwd
/home/JX/12345678A
```
El directorio padre de mi directorio HOME es /JX.

## Ejercicio 2
En el directorio **bin** hijo del **raíz** se pueden encontrar los ficheros que contienen los programas (ficheros ejecutables) asociados a una serie de comandos. Indique los **permisos**, **nombre de propietario**, **nombre del grupo de usuarios asignado**, **tamaño** y **fecha y hora** de creación de los ficheros correspondientes a los siguientes comandos: **cat** y **cp**

```bash
$ ls -lh /bin/cat /bin/cp
-rwxr-xr-x 1 root root  39K abr  5 16:36 /bin/cat
-rwxr-xr-x 1 root root 139K abr  5 16:36 /bin/cp
```
> El parametro `-l` de `ls` nos permite ver los detalles de los ficheros, `-h` nos permite ver el tamaño de los ficheros en un formato más legible para humanos.


## Ejercicio 3
Sitúe el directorio `etc` hijo del raíz como su directorio actual. Sin moverse de él:

```bash
$ cd /etc
```

**a)** Liste, usando **ruta absoluta** y **relativa**, el directorio raíz.

```bash
$ ls /
...root-files...
$ ls ..
...root-files...
```

**b)** Liste todos los ficheros y directorios del directorio actual (`/etc`) cuyo nombre empiece por la letra `n` y acabe en `conf`.

```bash
$ ls n*conf
nftables.conf nsswitch.conf
```

**c)** Liste, usando **ruta absoluta** y **ruta relativa**, el contenido del directorio lib hijo de `/var`, que contengan en su nombre la letra `a` en cualquier posición.

```bash
$ ls /var/lib/*a*
$ ls ../lib/*a*
...files-with-a...
```


## Ejercicio 4
Cree los directorios `dir1`, `dir2` y `tmp` en su directorio `HOME` con una **sola línea** de comandos. Vaya al directorio `dev` (hágalo su directorio de trabajo) que es **hijo del raíz** y **borre** `dir2` y `tmp` desde éste también con una **sola línea** de comandos. (**No** está permitido el uso **;** para esta pregunta).

```bash
$ mkdir dir1 dir2 tmp # Crear los directorios
$ cd /dev # Movemos al directorio dev
$ rm ~/dir2 ~/tmp # Borramos los directorios desde el directorio dev poniendo "~" para indicar el directorio "HOME"
```


## Ejercicio 5
**a)** **Copie** el fichero **passwd** que se encuentra en `/etc` a su directorio `HOME` con el nombre `mipasswd`. El fichero **originalmente** pertenece al usuario **root**, ¿quién es el **propietario** y a qué **grupo** ha sido asignada la **copia realizada**?

```bash
$ cd # Nos movemos a nuestro directorio HOME
$ cp /etc/passwd mipasswd
$ ls -l mipasswd
-rw-r--r-- 1 12345678A JX  10235 sep 11 11:48 mipasswd
```
> El nuevo propietario del fichero es el usuario con el nombre de usuario 12345678A y el grupo JX.

**b)** Copie, con una **única línea** de comandos, los ficheros `cp` y `cat` que se encuentran en el
directorio `/bin` a su directorio `HOME`.

```bash
$ cd # Nos movemos a nuestro directorio HOME
$ cp /bin/cp /bin/cat . # Copiamos los ficheros cp y cat al directorio "HOME"
```
> El comando **CP** toma el último argumento como el directorio de destino, mientras que el resto de argumentos son los ficheros que se copiarán. El directorio `.` es una abreviatura para el directorio actual.

**c)** Muestre por pantalla el contenido del fichero mipasswd.

```bash
$ cd # Nos movemos a nuestro directorio "HOME"
$ cat mipasswd
...mipasswd-content...
```

**d)** **Concatene** por pantalla (mostrar el contenido de **varios ficheros**, uno a continuación del contenido del anterior) el contenido de los ficheros `mipasswd`, `/etc/fstab`, y `/etc/netconfig`, usando una **única línea** de comandos.

```bash
$ cd # Nos movemos a nuestro directorio "HOME"
$ cat mipasswd /etc/fstab /etc/netconfig # Concatenamos los ficheros con el comando cat
...mipasswd-content...
...fstab-content...
...netconfig-content...
```


## Ejercicio 6
Dados los nombres de ficheros siguientes:

<table border="1" style="border-collapse: collapse;">
  <tr>
    <td>almanaque.1990</td>
    <td>almanaque.marzo.95</td>
    <td>almanaque.pas.283</td>

  </tr>
  <tr>
    <td>albaranes.1992</td>
    <td>albaranes.93</td>
    <td>malba.txt.98</td>
  </tr>
</table>

Busque patrones que engloben **sólo** los **ficheros** de cada **columna** y cada **fila** utilizando los metacaracteres `*` y `?`.

```bash
$ ls almanaque.*
almanaque.1990 almanaque.marzo.95 almanaque.pas.283

$ ls *alb*
albaranes.1992 albaranes.93 malba.txt.98

$ ls al*.19??
almanaque.1990 albaranes.1992

$ ls al*.9?
almanaque.marzo.95 albaranes.93

$ ls *.?8*
almanaque.pas.283 malba.txt.98
```


## Ejercicio 7

**a)** Establezca un enlace entre el fichero de texto **mipasswd** creado en el **ejercicio 5**, con otro que llamará `ejemplo_enlace.txt`, y ubíquelo dentro del directorio `dir1`, directorio creado en el **ejercicio 4**. Vea el contenido del directorio `HOME` ¿cómo o dónde se muestra el enlace realizado?

```bash
$ cd # Nos movemos a nuestro directorio "HOME"
$ ln mipasswd dir1/ejemplo_enlace.txt
$ ls -l
-rw-r--r--  2 12345678A JX     31 sep 26 10:28 mipasswd
```
> El enlace se muestra en el listado de ficheros con un número de enlaces mayor a 1, en este caso 2.

**b)** Cree en su directorio `HOME`, por ejemplo mediante una operación copia o usando el editor **vi**, un fichero de texto llamado `ejemplo2.txt`. Ahora, en vez de **enlazarlo** como antes, **copie** ese fichero y ubique el fichero destino en `dir1` con nombre ejemplo2_copia.txt.

```bash
$ cd # Nos movemos a nuestro directorio "HOME"
$ cp mipasswd ejemplo2.txt # Copiamos el fichero mipasswd a ejemplo2.txt
$ cp ejemplo2.txt dir1/ejemplo2_copia.txt # Copiamos el fichero ejemplo2.txt a dir1 con el nombre ejemplo2_copia.txt
```

**c)** Mediante el editor **vi** modifique (por ejemplo, simplemente borrando un caracter) el contenido de los **ficheros** `ejemplo_enlace.txt` y `ejemplo2_copia.txt`. Compare el contenido de éstos con el de `ejemplo.txt` y `ejemplo2.txt`, respectivamente. Razone el resultado de la comparación propuesta ¿cuál de estos últimos también ha variado su contenido?, ¿por qué?

```bash
$ vi dir1/ejemplo_enlace.txt # Modificamos el contenido del fichero ejemplo_enlace.txt
$ vi dir1/ejemplo2_copia.txt # Modificamos el contenido del fichero ejemplo2_copia.txt
$ cat ejemplo.txt # Mostramos el contenido del fichero ejemplo_enlace.txt
$ cat ejemplo2.txt # Mostramos el contenido del fichero ejemplo2_copia.txt
```
> El contenido de `ejemplo_enlace.txt` y `ejemplo2_copia.txt` ha variado, ya que ambos ficheros apuntan al mismo contenido en disco. Por otro lado, `ejemplo.txt` y `ejemplo2.txt` no han variado, ya que son ficheros independientes.

**d)** Borre, usando una única línea de comandos, los ficheros `ejemplo_enlace.txt`, `ejemplo2_copia.txt` y `ejemplo2.txt`. ¿qué parte/s del listado largo del directorio `HOME` ha/n cambiado?

```bash
$ rm dir1/ejemplo_enlace.txt dir1/ejemplo2_copia.txt
$ ls -l
-rw-r--r--  1 12345678A JX     31 sep 26 10:28 mipasswd
```
> La parte del listado largo del directorio `HOME` que ha cambiado es el número de links de los ficheros `mipasswd` y `ejemplo2.txt`, que ha pasado de 2 a 1.


