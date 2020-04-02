/*    /secure/SimulEfun/files.c
 *    from Nightmare 3.3
 *    functions which handle file operations
 *    created by Descartes of Borg 940926
 */

int file_exists(string str) { return (file_size(str) > -1); }

int create_dir(string str) {
    string *folders, dir = "";
    int created = 0;

    if(!str || file_size(str) == -2) return 0;
    folders = path_file(str);
    folders = explode(folders[0], "/") + (!regexp(folders[1], "\\.") ? ({ folders[1] }) : ({}));

    for(int i = 0; i < sizeof(folders); i ++) {
        dir = dir + "/" + folders[i];
        if(file_size(dir) == -1) created += unguarded((: mkdir, dir :));
    }
    return created;
}