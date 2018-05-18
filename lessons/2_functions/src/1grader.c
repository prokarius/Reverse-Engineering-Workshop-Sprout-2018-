char grader(int test, int exam){
    int total = test + exam;
    if (total >= 50){
        return 'P';
    }
    else {
        return 'F';
    }
}

int main(){
    int testscore = 26;
    int examscore = 22;
    char grade = grader(testscore, examscore);

    return 0;
}

