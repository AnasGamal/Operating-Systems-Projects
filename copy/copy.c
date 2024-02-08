#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

void copyFile(const char* src, const char* dest) {
	int src_fd = open(src, O_RDONLY);
	struct stat statBuf;
	stat(src, &statBuf);
	
	int dest_fd = open(dest, O_WRONLY | O_CREAT, statBuf.st_mode);

	if (src_fd == -1) {
        printf("Error opening source file or directory.\n");
        return;
    }

	if (dest_fd == -1) {
		printf("Error creating destination file.\n");
		close(src_fd);
		return;
    }

	char buf[1024];
	int nbytes;

	while ((nbytes = read(src_fd, buf, 1024)) > 0 ) {
		write(dest_fd, buf, nbytes);
	}
	close(src_fd);
	close(dest_fd);
	printf("Successfully copied file.\n");
}

void copyDir(const char* src, const char* dest) {
	DIR* dir = opendir(src);
	if (dir == NULL) {
		printf("Error opening source file or directory.\n");
		return;
    }
	struct dirent* entry;

	mkdir(dest, 0755);

	while((entry = readdir(dir)) != NULL) {
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
			char srcPath[1024];
			char destPath[1024];
			sprintf(srcPath, "%s/%s", src, entry->d_name);
			sprintf(destPath, "%s/%s", dest, entry->d_name);

			struct stat statBuf;
			
			stat(srcPath, &statBuf);

			if(S_ISDIR(statBuf.st_mode)) {
				copyDir(srcPath, destPath);
			} else {
				copyFile(srcPath, destPath);
			}
		}
	}

	closedir(dir);
	printf("Successfully copied directory.\n");
}

void copyItem(const char* src, const char* dest, int itemType) {
	if (itemType == DT_DIR) {
		printf("Copying directory...\n");
		copyDir(src, dest);
	} else {
		printf("Copying file...\n");
		copyFile(src, dest);
	}
}

int main(int argc, char* argv[]) {
	printf("## Copy Program ##\n");
	if (argc != 3) {
		printf("Insufficient arguments.\n");
		return -1;
	}
	const char* src = argv[1];
	const char* dest = argv[2];
	DIR* dir = opendir(src);
	if (dir != NULL) {
		closedir(dir);
		copyItem(src, dest, DT_DIR);
	} else {
		copyItem(src, dest, DT_REG);
	}
	printf("####\n");
	return 0;
}
