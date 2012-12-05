
typedef struct INPUT_TYPE *INPUT;

typedef struct INPUT_TYPE{
	double *X;
	int label;
}INPUT_TYPE;

typedef struct SEQUENCE_TYPE *SEQUENCE;

typedef struct SEQUENCE_TYPE{
	int inputs_no;
	INPUT *input;
}SEQUENCE_TYPE;


typedef struct INPUT_GROUP_TYPE *INPUT_GROUP;

typedef struct INPUT_GROUP_TYPE{
  int D;
  int sequences_no;
  int categories_no;
  SEQUENCE *sequence;
  char **label;
} INPUT_GROUP_TYPE;

INPUT_GROUP INPUT_GROUP_getInput(char *path);
INPUT_GROUP INPUT_GROUP_getInputLabel(char *path, char *pathLabel, int categories_no);
void INPUT_GROUP_norm(INPUT_GROUP input_group);

void INPUT_GROUP_free(INPUT_GROUP input_group);

void INPUT_GROUP_swaprandom(INPUT_GROUP input_group);
