# APUNTES UNIX FUNDAMENTOS DE SISTEMAS OPERATIVOS

## 1. Introducción a UNIX
Unix es un sistema operativo **multiusuario** y **multitarea**.
- Usable por varias personas al mismo tiempo.
- Capacidad de ejecutar varios programas al mismo tiempo.

Su estructura se basa en el **kernel** y el **shell**.
- **Kernel**: Núcleo del sistema operativo, todos los programas que controlan el acceso al ordenador, manejan su memoria y asignan recursos a los usuarios.
- **Shell**: Interfaz de usuario que permite la comunicación con el sistema operativo.
> [!TIP] 
> El **shell** puede ser **gráfico**, como el de Windows, o de **línea de comandos**, como el de UNIX.


## 2. Sesión en UNIX
Para acceder a un sistema UNIX, se puede hacer de dos formas:
1. **Localmente**: A través de una terminal.
2. **Remotamente**: A través de una conexión de red con el protocolo **SSH**.

> [!TIP] Util
> La tecla tabulador permite autocompletar comandos y nombres de ficheros.
> 
> ---
> 
> La tecla de flecha arriba permite recuperar comandos anteriormente ejecutados.

## 3. Ficheros y directorios
Tamibén conocidos como **archivos** y **carpetas**, contienen información y se organizan en una **estructura de árbol** con un nombre único.

> [!WARNING] Cuidado
> Los nombres de ficheros y directorios son **case sensitive** (distinguen entre mayúsculas y minúsculas).
>
> ---
>
> Los caracteres pueden ser cualquiera menos los siguientes que tienen un significado especial: `*`, `?`, `[`, `]`, `’`, `’’`, `` ` ``, `^`, `|`, cualquier car´acter ASCII inferior al 32 y superior al 127

> [!TIP] Util
> No existe una regla, pero si convenciones normalmente aceptadas, para nombrar ficheros y directorios. Por ejemplo, los ficheros de texto suelen tener la extensión `.txt` y los ficheros Python `.py`.
>
> ---
>
> Los ficheros que empiezan por `.` son ocultos.

## 4. Organización de ficheros y directorios
El sistema de ficheros de UNIX se organiza en una **estructura de árbol** con un **directorio raíz** `/` y varios **subdirectorios**.

> [!TIP] Util
> Relación **padre-hijo**: Un directorio puede contener otros directorios y ficheros, a los que se llama **hijos**. A su vez, un directorio puede ser contenido por otro directorio, al que se llama **padre**.

El directorio actual o de trabajo es el directorio en el que se encuentra el usuario en un momento dado. Se le conoce con el nombre `.` y el directorio padre con el nombre `..`.

El directorio `HOME` es el directorio personal del usuario, que se representa con el carácter `~` (virgulilla). Al iniciar sesión, el usuario se sitúa en su directorio `HOME` y es donde puede gestionar sus ficheros y directorios.

> [!TIP] Util
> Los ficheros pueden ser especificados de dos formas:
> - **Ruta absoluta**: Se especifica desde el directorio raíz `/`.
> - **Ruta relativa**: Se especifica desde el directorio actual.

## 5. Comandos básicos
Sintaixs de un comando:
```bash
nombre_comando [opciones] [argumentos]
```

### 5.1. Comandos básicos - Directorios

| Comando | Descripción |
| ------- | ----------- |
| `pwd` | Muestra el directorio actual. |
| `ls` | Lista los ficheros y directorios del directorio actual. |
| `cd` | Cambia de directorio. |
| `mkdir` | Crea un directorio. |
| `rmdir` | Elimina un directorio. |
| `rm` | Elimina un fichero o directorio. |
| `cat` | Muestra el contenido de un fichero. |
| `more` | Muestra el contenido de un fichero paginado. |
| `cp` | Copia un fichero o directorio. |
| `mv` | Mueve un fichero o directorio. |
| `cmp` | Compara dos ficheros byte a byte. |
| `touch` | Crea un fichero vacío. También sirve para cambiar la marca de tiempo de un fichero. |
| `ln` | Crea enlaces a ficheros. |

#### PWD
El comando `pwd` muestra el directorio actual.

```bash
$ pwd
/home/usuario
```

#### LS
El comando `ls` lista los ficheros y directorios del directorio actual o del directorio que especifiques. Tiene varias opciones:
- `-l`: Muestra información adicional.
- `-a`: Muestra todos los ficheros y directorios, incluidos los ocultos.
- `-h`: Muestra los tamaños de los ficheros y directorios en formato legible.
- `-d`: Muestra la información de los directorios, no de su contenido.
- `-1`: Muestra los ficheros y directorios en una sola columna.

```bash
$ ls
fichero1.txt fichero2.txt directorio1 directorio2
$ ls -l
-rw-r--r-- 1 usuario grupo 0 ene 1 00:00 fichero1.txt
-rw-r--r-- 1 usuario grupo 0 feb 14 02:34 fichero2.txt
drwxr-xr-x 2 usuario grupo 4096 mar 3 12:00 directorio1
```

#### CD
El comando `cd` cambia de directorio. Tiene varias opciones:
- `..`: Cambia al directorio padre.
- `~` o SIN ARGUMENTOS: Cambia al directorio HOME.

```bash
$ pwd
/home/usuario
$ cd directorio1
$ pwd
/home/usuario/directorio1
$ cd ..
$ pwd
/home/usuario
$ cd ~
$ pwd
/home/usuario
```

#### MKDIR
El comando `mkdir` crea un directorio.

- `-p` o `--parents`: Crea directorios intermedios si no existen.

```bash
$ mkdir directorio3
$ ls
fichero1.txt fichero2.txt directorio1 directorio2 directorio3
$ mkdir -p directorio4/directorio5
$ ls
fichero1.txt fichero2.txt directorio1 directorio2 directorio3 directorio4
$ ls directorio4
directorio5
```

#### RMDIR
El comando `rmdir` elimina un directorio **vacio**.

```bash
$ rmdir directorio3
$ ls
fichero1.txt fichero2.txt directorio1 directorio2 directorio4
```

#### RM
El comando `rm` elimina un fichero o directorio. Tiene varias opciones:
- `-r` o `--recursive`: Elimina un directorio y su contenido.
- `-f` o `--force`: No pide confirmación.
- `-i` o `--interactive`: Pide confirmación.

```bash
$ rm fichero2.txt
$ ls
fichero1.txt directorio1 directorio2 directorio4
$ rm -r directorio4
$ ls
fichero1.txt directorio1 directorio2
```

#### CAT
El comando `cat` muestra el contenido de un fichero.

- `-n` o `--number`: Muestra el número de línea.
- `-b` o `--number-nonblank`: Muestra el número de línea, pero no en las líneas en blanco.

```bash
$ cat fichero1.txt
Hola, mundo!
$ cat -n fichero2.txt
1 Hola, mundo!
2 Adiós, mundo!
```

#### MORE
El comando `more` muestra el contenido de un fichero paginado (probar en la terminal).

```bash
$ more fichero1.txt
Hola, mundo!
```

#### CP
El comando `cp` copia un fichero o directorio. Tiene varias opciones:
- `-r` o `--recursive`: Copia un directorio y su contenido.
- `-i` o `--interactive`: Pide confirmación.

```bash
$ cp fichero1.txt fichero3.txt
$ ls
fichero1.txt fichero3.txt directorio1 directorio2
$ cp -r directorio1 directorio6
$ ls
fichero1.txt fichero3.txt directorio1 directorio2 directorio6
```

#### MV
El comando `mv` mueve un fichero o directorio. También se puede utilizar para renombrar un fichero o directorio.

```bash
$ mv fichero3.txt fichero4.txt
$ ls
fichero1.txt fichero4.txt directorio1 directorio2 directorio6
$ mv directorio6 directorio7
$ ls
fichero1.txt fichero4.txt directorio1 directorio2 directorio7
```

#### CMP
El comando `cmp` compara dos ficheros byte a byte.

```bash
$ cmp fichero1.txt fichero4.txt
fichero1.txt fichero4.txt son iguales
```

#### TOUCH
El comando `touch` crea un fichero vacío. También sirve para cambiar la marca de tiempo de un fichero.

```bash
$ touch fichero5.txt
$ ls
fichero1.txt fichero4.txt fichero5.txt directorio1 directorio2 directorio7
```

#### LN
El comando `ln` crea enlaces a ficheros. Tiene dos tipos de enlaces:
- **Enlace duro**: Crea un nuevo fichero que apunta al mismo contenido que el original. Si se elimina el original, el enlace duro sigue existiendo.
- **Enlace simbólico**: Crea un nuevo fichero que apunta al original. Si se elimina el original, el enlace simbólico deja de funcionar.

```bash
$ ln fichero1.txt fichero6.txt
$ ls
fichero1.txt fichero4.txt fichero5.txt fichero6.txt directorio1 directorio2 directorio7
$ ln -s fichero1.txt fichero7.txt
$ ls
fichero1.txt fichero4.txt fichero5.txt fichero6.txt fichero7.txt directorio1 directorio2 directorio7
```

### 5.2. Comandos básicos - Permisos

| Comando | Descripción |
| ------- | ----------- |
| `chmod` | Cambia los permisos de un fichero o directorio. |

#### CHMOD
El comando `chmod` cambia los permisos de un fichero o directorio. Los permisos se dividen en tres grupos:
- **Propietario**: Usuario que creó el fichero o directorio.
- **Grupo**: Conjunto de usuarios que tienen los mismos permisos.
- **Otros**: Resto de usuarios.

**Permisos**: Lectura (`r`), escritura (`w`) y ejecución (`x`).

Dos formas de especificar los permisos:

**Numérica**: Se especifica un número de tres dígitos, donde cada dígito representa un grupo de permisos en octal.
  
- Ejemplo: `rwx` (111) = 7, `rw-` (110) = 6, `r-x` (101) = 5, `r--` (100) = 4, `---` (000) = 0.

```bash
$ ls -l
-rw-r--r-- 1 usuario grupo 0 ene 1 00:00 fichero1.txt
$ chmod 755 fichero1.txt
$ ls -l
-rwxr-xr-x 1 usuario grupo 0 ene 1 00:00 fichero1.txt
```

**Simbólica**: Se especifica el grupo de permisos y el operador (`+`, `-`, `=`) seguido de los permisos.

- Ejemplo: `u+x` (Añade permiso de ejecución al propietario), `g-w` (Quita permiso de escritura al grupo), `a-r` (Quita permiso de lectura a todos).

```bash
$ ls -l
-rw-r--r-- 1 usuario grupo 0 ene 1 00:00 fichero1.txt
$ chmod u+x fichero1.txt
$ ls -l
-rwxr--r-- 1 usuario grupo 0 ene 1 00:00 fichero1.txt
```

### 5.3. Comandos básicos - Utilidades

| Comando | Descripción |
| ------- | ----------- |
| `exit` | Cierra la sesión. |
| `man` | Muestra el manual de un comando. |
| `passwd` | Cambia la contraseña del usuario. |
| `who` | Muestra los usuarios conectados. |
| `whoami` | Muestra el nombre del usuario. |
| `clear` | Limpia la pantalla. |
| `date` | Muestra la fecha y hora actual. |
| `echo` | Muestra un mensaje que se pase por argumento. |

#### EXIT
El comando `exit` cierra la sesión.

```bash
$ exit
```

#### MAN
El comando `man` muestra el manual de un comando.

```bash
$ man ls
LS(1)                                               
NAME
       ls - list directory contents

SYNOPSIS
       ls [OPTION]... [FILE]...

DESCRIPTION
List  information  about the FILEs (the current directory by default). Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.
```

#### PASSWD
El comando `passwd` cambia la contraseña del usuario.

```bash
$ passwd
Changing password for usuario.
(current) UNIX password:
Enter new UNIX password:
Retype new UNIX password:
passwd: password updated successfully
```

#### WHO
El comando `who` muestra los usuarios conectados.

```bash
$ who
usuario pts/0        2024-09-28 13:55 (xx.xx.xx.xx)
```

#### WHOAMI
El comando `whoami` muestra el nombre del usuario.

```bash
$ whoami
usuario
```

#### CLEAR
El comando `clear` limpia la pantalla.

```bash
$ clear
```

#### DATE
El comando `date` muestra la fecha y hora actual.

```bash
$ date
Wed Sep 28 13:55:00 CEST 2024
```

#### ECHO
El comando `echo` muestra un mensaje que se pase por argumento.

```bash
$ echo Hola, mundo!
Hola, mundo!
```

#### FIND
El comando `find` busca ficheros y directorios en un árbol de directorios. Tiene varias opciones:
- `-name`: Busca por nombre.
- `-type`: Busca por tipo (fichero o directorio).
- `-exec`: Ejecuta un comando en los ficheros encontrados.

```bash
$ find / -name fichero1.txt
/home/usuario/fichero1.txt
$ find / -type f
/home/usuario/fichero1.txt
$ find / -type d
/home/usuario
```

## 6 Metacaracteres
Los metacaracteres son caracteres especiales que tienen un significado especial en UNIX. Se pueden usar con los comandos para realizar búsquedas más avanzadas.

| Metacaracter | Descripción |
| ------- | ----------- |
| `*` | Cualquier cadena de caracteres. |
| `?` | Cualquier carácter. |

### 6.1. Metacaracteres - Asterisco
El metacaracter `*` representa cualquier cadena de caracteres de 0 o más longitud.

```bash
$ ls
fichero.txt fichero2.txt fichero3.txt
$ ls fichero*
fichero1.txt fichero2.txt fichero3.txt
```

### 6.2. Metacaracteres - Interrogación
El metacaracter `?` representa cualquier carácter.

```bash
$ ls
fichero.txt fichero2.txt fichero3.txt
$ ls fichero?.txt
fichero2.txt fichero3.txt
```

## 7. Entrada y salida en la ejecución de comandos
Los comandos en UNIX pueden recibir **entrada** y producir **salida**.

**Entrada de un comando**: Forma en la que recibes los datos de un comando.
- **Entrada estándar**: Se recibe a través del teclado.

**Salida de un comando**: Forma en la que se muestran los resultados de un comando.
- **Salida estándar**: Se muestra en la pantalla (terminal).
  
**Salida de errores de un comando**: Forma en la que se muestran los errores de un comando.
- **Salida de errores estándar**: Se muestra en la pantalla (terminal).

> [!WARNING] Cuidado
> No todos los **comandos** tienen **entrada** y/o **salida**.
>
> ---
>
> **Todos** los **comandos** tienen **salida estandar** de **errores**.

## 8 Comandos avanzados
Existen comandos más avanzados que permiten realizar tareas más complejas.

| Comando | Descripción |
| ------- | ----------- |
| `sort` | Ordena líneas de texto. |
| `grep` | Busca patrones en un fichero. |
| `uniq` | Elimina lineas de texto seguidas iguales. |
| `head` | Muestra las primeras líneas de un fichero. |
| `tail` | Muestra las últimas líneas de un fichero. |
| `paste` | Combina líneas de texto. |
| `wc` | Cuenta líneas, palabras y caracteres. |
| `tr` | Traduce o elimina caracteres. |
| `cut` | Selecciona columna/s de la entrada, mostrando el resultado por la salida estándar. |
| `scp` | Copia ficheros de forma segura entre hosts. |