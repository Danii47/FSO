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


## Ejercicio 8
**a)** Cambie de nombre al fichero `mipasswd` creado en el **ejercicio 5**, y llámelo `texto.txt`. Haga una copia de esta fichero y llámela `texto_cop.txt`.

```bash
$ cd # Nos movemos a nuestro directorio "HOME"
$ mv mipasswd texto.txt # Cambiamos el nombre del fichero mipasswd a texto.txt
$ cp texto.txt texto_cop.txt # Copiamos el fichero texto.txt a texto_cop.txt
```

**b)** Modifique los **permisos** del fichero `texto.txt` de manera que se **elimine** el **permiso de escritura** para el **propietario**. Intente **borrar** el fichero. Como está **protegido** contra **escritura**, significa que **no** se puede **modificar**, lo que incluye el **borrado**. Sin embargo, como el fichero es **suyo** le pregunta si quiere **borrarlo** (si no fuera suyo, el sistema **directamente impediría el borrado**). Responda **no** a la pregunta. Bórrelo nuevamente pero usando la opción de forzar el borrado para que no pregunte.

```bash
$ chmod u-w texto.txt # Eliminamos el permiso de escritura para el propietario
$ rm texto.txt # Intentamos borrar el fichero
rm: remove write-protected regular file 'texto.txt'? n # Respondemos no a la pregunta
$ rm -f texto.txt # Forzamos el borrado del fichero
```


## Ejercicio 9
**a)** Cambie el nombre al directorio `dir1`, y llámelo `tmp`.

```bash
$ mv dir1 tmp # Cambiamos el nombre del directorio dir1 a tmp
```

**b)** Cree un fichero en el directorio `tmp`. ¿Se podría borrar el directorio con el comando rmdir? Bórrelo usando una sola línea de comandos.

```bash
$ touch tmp/fichero.txt # Creamos un fichero en el directorio tmp
$ rmdir tmp # Intentamos borrar el directorio tmp
rmdir: failed to remove 'tmp': Directory not empty # No se puede borrar el directorio porque no está vacío
$ rm tmp/fichero.txt; rmdir tmp # Borramos el fichero y el directorio en una sola línea de comandos
# También se puede usar rm -r tmp para borrar el directorio y su contenido
```
> El comando `rmdir` solo puede borrar directorios vacíos.
> 
> El comando `rm -r` borra el directorio y su contenido.


## Ejercicio 10
Copie a su directorio HOME el fichero `/home/dieggar/tmp/texto_cop.txt`:

```bash
$ cp /home/dieggar/tmp/texto_cop.txt ~ # Copiamos el fichero texto cop.txt a nuestro directorio "HOME"
```

**a)** El fichero `/home/dieggar/tmp/texto_cop.txt`, pertenece **originalmente** al usuario
**dieggar**, ¿quién es el **propietario** y a qué **grupo** ha sido asignada la **copia** realizada?

```bash
$ ls -l /home/dieggar/tmp/texto_cop.txt
-rw-r--r-- 1 dieggar dieggar 31 sep 26 10:28 /home/dieggar/tmp/texto_cop.txt
$ ls -l texto_cop.txt
-rw-r--r-- 1 12345678A JX 31 sep 26 10:28 texto_cop.txt
```
> El nuevo propietario del fichero es el usuario con el nombre de usuario 12345678A y el grupo JX, el usuario que copia el fichero.

**b)** Antes de la operación copia, ya existía en su `HOME`, tras la primera operación indicada en el **ejercicio 8**, un fichero llamado `texto_cop.txt`, ¿qué ha pasado con él tras la copia? ¿Cómo podríamos **proteger** un **fichero** frente a **copias** con el mismo **nombre** realizadas por **error**?

```bash
$ ls -l texto_cop.txt
-rw-r--r-- 1 12345678A JX 31 sep 26 10:28 texto_cop.txt
```
> El fichero `texto_cop.txt` ha sido **sobrescrito** por la copia realizada.
> 
> Para proteger un fichero frente a copias con el mismo nombre realizadas por error, se puede **cambiar el nombre** del fichero **antes** de realizar la **copia**. También **quitar** los **permisos** de **escritura** de nuestro fichero, para **evitar** que se pueda **sobrescribir** por **error**.


## Ejercicio 11
---


## Ejercicio 12
Vaya al directorio `/sys` y **ejecute** el comando `ls d*` o `ll d*`. Teóricamente todos los elementos listados (**ficheros** o **directorios**) deberían empezar por `d`, pero no es así. ¿Por qué? Busque entre las opciones de `ls` aquella que permita listar del contenido del directorio `/sys`, las entradas correspondientes a sus **subdirectorios** `dev` y `devices`, no su contenido.

```bash
$ cd /sys
$ ls -d d*
devices dev
```
> El comando `ls` tiene una opción `-d` que permite listar los directorios sin mostrar su contenido.


## Ejercicio 13
**a)** Cree, mediante una única línea de comandos, tres directorios llamados `dir1`, `dir2` y `dir3` de manera que `dir1` cuelgue del `HOME`, `dir1` sea padre de `dir2` y `dir2` sea padre de `dir3`. Busque la opción.

```bash
$ mkdir dir1 dir1/dir2 dir1/dir2/dir3
# Usando la opción -p de mkdir, se pueden crear directorios anidados
$ mkdir -p dir1/dir2/dir3
```
> La opción `-p` de `mkdir` permite crear directorios anidados.

**b)** Sitúe como directorio actual el raíz. Sin moverse de ese directorio, copie el fichero `texto.txt` que está en su `HOME` en el directorio `dir1`.

```bash
$ cd /
$ cp ~/texto.txt ~/dir1 # Si no se especifica el nombre del fichero, se copia con el mismo nombre
```

**c)** Desde `dir1` (hágalo su directorio actual), **mueva** el fichero `texto.txt` que está en el `HOME` al directorio `dir2` con el nombre de `texto_cop_2.txt` usando **rutas relativas**.

```bash
$ cd ~/dir1
$ mv ../texto.txt dir2/texto_cop_2.txt # Movemos el fichero texto.txt al directorio dir2 con el nombre texto_cop_2.txt
```
> Con `..` se puede acceder al directorio padre.

**d)** Desde `dir3` (hágalo su directorio actual), **mueva** el fichero `texto_cop_2.txt` del directorio `dir2` al directorio actual usando **rutas relativas**.

```bash
# Estamos en dir1 por el paso anterior
$ cd dir2/dir3
$ mv ../texto_cop_2.txt . # Movemos el fichero texto_cop_2.txt al directorio actual
```

**e)** **Borre**, mediante una **única línea** de comandos, el directorio `dir2` y todo su contenido.

```bash
$ cd ~/dir1
$ rm -rf dir2 # Borramos el directorio dir2 y todo su contenido
```
> El comando `rm -r` borra el directorio y su contenido, mientras que la opción `-f` evita que se muestren mensajes de confirmación para cada fichero borrado.


## Ejercicio 14
Muestre el contenido del fichero `/etc/netconfig` mostrando el número de cada línea.

```bash
$ cat -n /etc/netconfig
     1  #
     2  # The network configuration file. This file is currently only used in
     3  # conjunction with the TI-RPC code in the libtirpc library.
     4  #
     5  # Entries consist of:
     6  #
     7  #       <network_id> <semantics> <flags> <protofamily> <protoname> \
     8  #               <device> <nametoaddr_libs>
     9  #
    10  # The <device> and <nametoaddr_libs> fields are always empty in this
    11  # implementation.
    12  #
    13  udp        tpi_clts      v     inet     udp     -       -
    14  tcp        tpi_cots_ord  v     inet     tcp     -       -
    15  udp6       tpi_clts      v     inet6    udp     -       -
    16  tcp6       tpi_cots_ord  v     inet6    tcp     -       -
    17  rawip      tpi_raw       -     inet      -      -       -
    18  local      tpi_cots_ord  -     loopback  -      -       -
    19  unix       tpi_cots_ord  -     loopback  -      -       -
```
> El comando `cat` tiene una opción `-n` que permite mostrar el número de cada línea.


## Ejercicio 15
Se llama **extensión** de un **fichero** a la **cadena de caracteres** que aparece **tras** el caracter `.` al final del nombre. Por ejemplo, el fichero `programa.c` tiene extensión `c` (que significa que **contiene** los **fuentes** de un programa en **lenguaje C**) y en el fichero `ejemplo.txt` la extensión es `txt`. **Liste** en **formato corto** (sólo nombres) el contenido del directorio `/etc`, de manera que la **salida** aparezca en **una columna** y **ordenada por la extensión**. Para ello, consulte las opciones del comando `ls`.

```bash
# SOLO CON LS USANDO LA OPCION -X
$ ls -1X /etc
...
updatedb.conf
vconsole.conf
xattr.conf
manpath.config
apparmor.d
bash_completion.d
binfmt.d
cgconfig.d
...
```
> El comando `ls` tiene una opción `-X` que permite ordenar los ficheros por extensión.
>
> La opción `-1` de `ls` permite mostrar los ficheros en una sola columna.