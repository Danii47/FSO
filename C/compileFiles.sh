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
            	echo -e "\033[1;31mError al compilar $file. Reintentando con -lm...\033[0m"
                gcc -o "${number}.out" "$file" -lm 2>/dev/null

                # Si vuelve a fallar, muestra un mensaje de error
                if [ $? -ne 0 ]; then
                    echo -e "\033[1;31mError al compilar $file incluso con -lm\033[0m"
                else
                    echo -e "\033[1;32mCompilado el archivo $file con -lm a ${number}.out\033[0m"
                fi
            else
                echo -e "\033[1;32mCompilado el archivo $file a ${number}.out\033[0m"
            fi
        fi
    done

    # Vuelve al directorio padre
    cd ..
done

# Subo los cambios a Git
if [[ "$1" == "y" || "$1" == "yes" ]]; then
    git add .
    commit_message="Commit automático de actualización carpeta C de FSO | $(date +"%Y-%m-%d %H:%M:%S")"
    git commit -m "$commit_message" > /dev/null 2>&1
    git push origin main > /dev/null 2>&1
    echo -e "\033[1;34mCambios subidos automaticamente al repositorio de GitHub.\nhttps://github.com/Danii47/FSO\033[0m"
else
    echo -e "\033[97mCambios \033[31mNO\033[97m subidos a GitHub.\033[0m"
fi

# Crear un archivo zip de la carpeta actual
tar --exclude="C/*/*.txt" -czf "$tar_file" -C "$HOME/FSO" "C"
echo -e "\033[1;34mCreado el comprimido: $tar_file\033[0m"
