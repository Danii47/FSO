C_DIR="$HOME/FSO/C"
tar_file="$HOME/C.tar.gz"

# Cambiar a la carpeta C en caso de existir
cd "$C_DIR" || { echo "No se pudo acceder a la carpeta $C_DIR"; exit 1; }

for dir in */; do
    cd "$dir" || continue

    for file in *.c; do
        # Comprueba si existe el archivo
        if [ -e "$file" ]; then
            # Obtiene el nombre del archivo (sin .c)
            number="${file%.c}"
            # Compila el archivo
            gcc -o "${number}.out" "$file" 2>/dev/null
	    if [ $? -ne 0 ]; then
            	echo "Error al compilar $file. Reintentando con -lm..."
                gcc -o "${number}.out" "$file" -lm 2>/dev/null

                # Si vuelve a fallar, muestra un mensaje de error
                if [ $? -ne 0 ]; then
                    echo "Error al compilar $file incluso con -lm"
                else
                    echo "Compilado el archivo $file con -lm a ${number}.out"
                fi
            else
                echo "Compilado el archivo $file a ${number}.out"
            fi
        fi
    done

    # Vuelve al directorio padre
    cd ..
done

# Subo los cambios a Git
git add .
git commit -m "Commit automático de actualización carpeta C de FSO."
git push origin main
echo "Cambios subidos automaticamente al repositorio de GitHub."

# Crear un archivo zip de la carpeta actual
tar -czf "$tar_file" -C "$HOME" "FSO" "C"
echo "Creado el comprimido: $tar_file"
