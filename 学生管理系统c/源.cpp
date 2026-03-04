//修改 
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 常量定义
#define STUDENT_FILE "students.txt"
#define TEACHER_FILE "teachers.txt"
#define ADMIN_FILE "admins.txt"
#define SCORE_FILE "scores.txt"
#define APPEAL_FILE "appeals.txt"

#define MAX_ID 10000
#define MAX_NAME 10000
#define MAX_PWD 10000
#define MAX_CLASS 10000
#define MAX_COURSE 10000
#define MAX_LINE 10000
#define MAX_REASON 10000

// 成绩结构体
typedef struct Score {
    char courseName[MAX_COURSE];
    double score;
    struct Score* next;
    struct Score* prev;
} Score;

// 申诉结构体
typedef struct Appeal {
    char studentId[MAX_ID];
    char courseName[MAX_COURSE];
    double oldScore;
    double newScore;
    char reason[MAX_REASON];
    int status;          // 0:待处理 1:已处理
    struct Appeal* next;
    struct Appeal* prev;
} Appeal;

// 学生结构体
typedef struct Student {
    char id[MAX_ID];
    char name[MAX_NAME];
    char password[MAX_PWD];
    char className[MAX_CLASS];
    double totalScore;
    double avgScore;
    int rank;
    Score* scoreHead;
    struct Student* next;
    struct Student* prev;
} Student;

// 教师结构体
typedef struct Teacher {
    char id[MAX_ID];
    char name[MAX_NAME];
    char password[MAX_PWD];
    char className[MAX_CLASS];
    struct Teacher* next;
    struct Teacher* prev;
} Teacher;

// 管理员结构体
typedef struct Admin {
    char id[MAX_ID];
    char password[MAX_PWD];
    struct Admin* next;
    struct Admin* prev;
} Admin;

// 全局链表头指针
Student* studentHead = NULL;
Teacher* teacherHead = NULL;
Admin* adminHead = NULL;
Appeal* appealHead = NULL;

// ==================== 工具函数 ====================
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void getPassword(char* pwd) {
    scanf("%s", pwd);
}

int getCourseCount(Student* s) {
    int count = 0;
    Score* sc = s->scoreHead;
    while (sc) {
        count++;
        sc = sc->next;
    }
    return count;
}

// ==================== 链表操作 ====================

// 学生

//创建新学生节点
Student* createStudent(const char* id, const char* name, const char* pwd, const char* className) {
    Student* s = (Student*)malloc(sizeof(Student));
    strcpy(s->id, id);
    strcpy(s->name, name);
    strcpy(s->password, pwd);
    strcpy(s->className, className);
    s->totalScore = 0;
    s->avgScore = 0;
    s->rank = 0;
    s->scoreHead = NULL;
    s->next = NULL;
    s->prev = NULL;
    return s;
}

//添加学生节点到链表
void addStudent(Student* s) {
    if (!studentHead) {
        studentHead = s;
        return;
    }
    Student* curr = studentHead;
    while (curr->next) curr = curr->next;
    curr->next = s;
    s->prev = curr;
}

//删除学生节点
void deleteStudent(const char* id) {
    Student* curr = studentHead;
    while (curr && strcmp(curr->id, id) != 0) curr = curr->next;
    if (!curr) return;

    if (curr->prev) curr->prev->next = curr->next;
    else studentHead = curr->next;
    if (curr->next) curr->next->prev = curr->prev;

    Score* sc = curr->scoreHead;
    while (sc) {
        Score* temp = sc;
        sc = sc->next;
        free(temp);
    }
    free(curr);
}

//查找学生
Student* findStudent(const char* id) {
    Student* curr = studentHead;
    while (curr) {
        if (strcmp(curr->id, id) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

// 教师
Teacher* createTeacher(const char* id, const char* name, const char* pwd, const char* className) {
    Teacher* t = (Teacher*)malloc(sizeof(Teacher));
    strcpy(t->id, id);
    strcpy(t->name, name);
    strcpy(t->password, pwd);
    strcpy(t->className, className);
    t->next = NULL;
    t->prev = NULL;
    return t;
}

void addTeacher(Teacher* t) {
    if (!teacherHead) {
        teacherHead = t;
        return;
    }
    Teacher* curr = teacherHead;
    while (curr->next) curr = curr->next;
    curr->next = t;
    t->prev = curr;
}

Teacher* findTeacher(const char* id) {
    Teacher* curr = teacherHead;
    while (curr) {
        if (strcmp(curr->id, id) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

void deleteTeacher(const char* id) {
    Teacher* curr = teacherHead;
    while (curr && strcmp(curr->id, id) != 0) curr = curr->next;
    if (!curr) return;

    if (curr->prev) curr->prev->next = curr->next;
    else teacherHead = curr->next;
    if (curr->next) curr->next->prev = curr->prev;
    free(curr);
}

// 管理员
Admin* createAdmin(const char* id, const char* pwd) {
    Admin* a = (Admin*)malloc(sizeof(Admin));
    strcpy(a->id, id);
    strcpy(a->password, pwd);
    a->next = NULL;
    a->prev = NULL;
    return a;
}

void addAdmin(Admin* a) {
    if (!adminHead) {
        adminHead = a;
        return;
    }
    Admin* curr = adminHead;
    while (curr->next) curr = curr->next;
    curr->next = a;
    a->prev = curr;
}

Admin* findAdmin(const char* id) {
    Admin* curr = adminHead;
    while (curr) {
        if (strcmp(curr->id, id) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

void deleteAdmin(const char* id) {
    Admin* curr = adminHead;
    while (curr && strcmp(curr->id, id) != 0) curr = curr->next;
    if (!curr) return;

    if (curr->prev) curr->prev->next = curr->next;
    else adminHead = curr->next;
    if (curr->next) curr->next->prev = curr->prev;
    free(curr);
}

// 申诉
Appeal* createAppeal(const char* stuId, const char* course, double oldScore, double newScore, const char* reason) {
    Appeal* a = (Appeal*)malloc(sizeof(Appeal));
    strcpy(a->studentId, stuId);
    strcpy(a->courseName, course);
    a->oldScore = oldScore;
    a->newScore = newScore;
    strcpy(a->reason, reason);
    a->status = 0;
    a->next = NULL;
    a->prev = NULL;
    return a;
}

void addAppeal(Appeal* a) {
    if (!appealHead) {
        appealHead = a;
        return;
    }
    Appeal* curr = appealHead;
    while (curr->next) curr = curr->next;
    curr->next = a;
    a->prev = curr;
}

void deleteAppeal(Appeal* a) {
    if (!a) return;
    if (a->prev) a->prev->next = a->next;
    else appealHead = a->next;
    if (a->next) a->next->prev = a->prev;
    free(a);
}

Appeal* findAppeal(const char* stuId, const char* course) {
    Appeal* curr = appealHead;
    while (curr) {
        if (strcmp(curr->studentId, stuId) == 0 && strcmp(curr->courseName, course) == 0 && curr->status == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

// ==================== 文件操作 ====================
void loadStudents() {
    FILE* fp = fopen(STUDENT_FILE, "r");
    if (!fp) return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD], className[MAX_CLASS];
        if (sscanf(line, "%s %s %s %s", id, name, pwd, className) != 4) continue;

        Student* s = createStudent(id, name, pwd, className);
        addStudent(s);

        // 加载成绩
        FILE* scoreFp = fopen(SCORE_FILE, "r");
        if (scoreFp) {
            char scoreLine[MAX_LINE];
            while (fgets(scoreLine, sizeof(scoreLine), scoreFp)) {
                scoreLine[strcspn(scoreLine, "\n")] = 0;

                char sid[MAX_ID], course[MAX_COURSE];
                double score;
                if (sscanf(scoreLine, "%s %s %lf", sid, course, &score) != 3) continue;

                if (strcmp(sid, id) == 0) {
                    Score* sc = (Score*)malloc(sizeof(Score));
                    strcpy(sc->courseName, course);
                    sc->score = score;
                    sc->next = NULL;
                    sc->prev = NULL;
                    if (!s->scoreHead) s->scoreHead = sc;
                    else {
                        Score* curr = s->scoreHead;
                        while (curr->next) curr = curr->next;
                        curr->next = sc;
                        sc->prev = curr;
                    }
                    s->totalScore += score;
                }
            }
            fclose(scoreFp);
        }

        int courseCount = getCourseCount(s);
        if (courseCount > 0) {
            s->avgScore = s->totalScore / courseCount;
        }
    }
    fclose(fp);
}

void saveStudents() {
    FILE* fp = fopen(STUDENT_FILE, "w");
    if (!fp) return;

    Student* curr = studentHead;
    while (curr) {
        fprintf(fp, "%s %s %s %s\n", curr->id, curr->name, curr->password, curr->className);
        curr = curr->next;
    }
    fclose(fp);
}

void saveScores() {
    FILE* fp = fopen(SCORE_FILE, "w");
    if (!fp) return;

    Student* s = studentHead;
    while (s) {
        Score* sc = s->scoreHead;
        while (sc) {
            fprintf(fp, "%s %s %.2lf\n", s->id, sc->courseName, sc->score);
            sc = sc->next;
        }
        s = s->next;
    }
    fclose(fp);
}

void loadTeachers() {
    FILE* fp = fopen(TEACHER_FILE, "r");
    if (!fp) return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD], className[MAX_CLASS];
        if (sscanf(line, "%s %s %s %s", id, name, pwd, className) != 4) continue;

        Teacher* t = createTeacher(id, name, pwd, className);
        addTeacher(t);
    }
    fclose(fp);
}

void saveTeachers() {
    FILE* fp = fopen(TEACHER_FILE, "w");
    if (!fp) return;

    Teacher* curr = teacherHead;
    while (curr) {
        fprintf(fp, "%s %s %s %s\n", curr->id, curr->name, curr->password, curr->className);
        curr = curr->next;
    }
    fclose(fp);
}

void loadAdmins() {
    FILE* fp = fopen(ADMIN_FILE, "r");
    if (!fp) return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char id[MAX_ID], pwd[MAX_PWD];
        if (sscanf(line, "%s %s", id, pwd) != 2) continue;

        Admin* a = createAdmin(id, pwd);
        addAdmin(a);
    }
    fclose(fp);
}

void saveAdmins() {
    FILE* fp = fopen(ADMIN_FILE, "w");
    if (!fp) return;

    Admin* curr = adminHead;
    while (curr) {
        fprintf(fp, "%s %s\n", curr->id, curr->password);
        curr = curr->next;
    }
    fclose(fp);
}

void loadAppeals() {
    FILE* fp = fopen(APPEAL_FILE, "r");
    if (!fp) return;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;

        char stuId[MAX_ID], course[MAX_COURSE], reason[MAX_REASON];
        double oldScore, newScore;
        int status;
        if (sscanf(line, "%s %s %lf %lf %s %d", stuId, course, &oldScore, &newScore, reason, &status) != 6)
            continue;

        Appeal* a = createAppeal(stuId, course, oldScore, newScore, reason);
        a->status = status;
        addAppeal(a);
    }
    fclose(fp);
}

void saveAppeals() {
    FILE* fp = fopen(APPEAL_FILE, "w");
    if (!fp) return;

    Appeal* curr = appealHead;
    while (curr) {
        fprintf(fp, "%s %s %.2lf %.2lf %s %d\n", curr->studentId, curr->courseName,
            curr->oldScore, curr->newScore, curr->reason, curr->status);
        curr = curr->next;
    }
    fclose(fp);
}

// ==================== 通用注册（主菜单用） ====================
void generalRegister() {
    printf("\n=== 通用账号注册 ===\n");
    int typeChoice;
    char type[10]; // "admin", "teacher", "student"
    char id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD], className[MAX_CLASS];

    while (1) {
        printf("请选择注册类型：\n");
        printf("1. 管理员账号（系统最高权限）\n");
        printf("2. 教师账号\n");
        printf("3. 学生账号\n");
        printf("0. 返回主菜单\n");
        printf("选择：");
        scanf("%d", &typeChoice);
        clearInputBuffer();

        if (typeChoice == 0) return;
        else if (typeChoice == 1) strcpy(type, "admin");
        else if (typeChoice == 2) strcpy(type, "teacher");
        else if (typeChoice == 3) strcpy(type, "student");
        else {
            printf("无效选择，请重新输入！\n");
            continue;
        }
        break;
    }

    // 输入ID并查重
    while (1) {
        printf("请输入账号ID：");
        scanf("%s", id);
        int exists = 0;
        if (strcmp(type, "admin") == 0 && findAdmin(id)) exists = 1;
        else if (strcmp(type, "teacher") == 0 && findTeacher(id)) exists = 1;
        else if (strcmp(type, "student") == 0 && findStudent(id)) exists = 1;

        if (exists) {
            printf("该ID已被注册，请更换ID！\n");
            continue;
        }
        break;
    }

    printf("请输入密码：");
    getPassword(pwd);

    if (strcmp(type, "admin") == 0) {
        Admin* a = createAdmin(id, pwd);
        addAdmin(a);
        saveAdmins();
        printf("管理员账号注册成功！ID：%s\n", id);
        return;
    }

    printf("请输入姓名：");
    scanf("%s", name);
    printf("请输入班级：");
    scanf("%s", className);

    if (strcmp(type, "teacher") == 0) {
        Teacher* t = createTeacher(id, name, pwd, className);
        addTeacher(t);
        saveTeachers();
        printf("教师账号注册成功！ID：%s 班级：%s\n", id, className);
    }
    else if (strcmp(type, "student") == 0) {
        Student* s = createStudent(id, name, pwd, className);
        addStudent(s);
        saveStudents();
        printf("学生账号注册成功！ID：%s 班级：%s\n", id, className);
    }
}

// ==================== 密码找回（主菜单） ====================
void passwordRecovery() {
    printf("\n=== 密码找回 ===\n");
    int type;
    char id[MAX_ID], name[MAX_NAME];
    printf("请选择账号类型：\n");
    printf("1. 学生\n");
    printf("2. 教师\n");
    printf("3. 管理员\n");
    printf("选择：");
    scanf("%d", &type);
    clearInputBuffer();

    printf("请输入账号ID：");
    scanf("%s", id);

    if (type == 1) {
        printf("请输入姓名：");
        scanf("%s", name);
        Student* s = findStudent(id);
        if (s && strcmp(s->name, name) == 0) {
            printf("您的密码是：%s\n", s->password);
        }
        else {
            printf("信息不匹配，无法找回密码。\n");
        }
    }
    else if (type == 2) {
        printf("请输入姓名：");
        scanf("%s", name);
        Teacher* t = findTeacher(id);
        if (t && strcmp(t->name, name) == 0) {
            printf("您的密码是：%s\n", t->password);
        }
        else {
            printf("信息不匹配，无法找回密码。\n");
        }
    }
    else if (type == 3) {
        Admin* a = findAdmin(id);
        if (a) {
            printf("您的密码是：%s\n", a->password);
        }
        else {
            printf("管理员ID不存在。\n");
        }
    }
    else {
        printf("无效类型。\n");
    }
}

// ==================== 学生端功能 ====================
void studentQueryScore(Student* s) {
    printf("\n=== 成绩查询 ===\n");
    if (!s->scoreHead) {
        printf("暂无成绩记录\n");
        return;
    }
    printf("学号：%s 姓名：%s 班级：%s\n", s->id, s->name, s->className);
    printf("-------------------------\n");
    Score* sc = s->scoreHead;
    while (sc) {
        printf("%s：%.2lf\n", sc->courseName, sc->score);
        sc = sc->next;
    }
    printf("总分：%.2lf 平均分：%.2lf\n", s->totalScore, s->avgScore);
}

// 比较函数用于排序学生（按总分降序）
int compareStudentByTotalScore(const void* a, const void* b) {//void* 是「通用指针类型」，可以接收任意类型指针的赋值（比如 int*、Student*、char*），不用为每种类型单独写回调函数，实现通用性。
    Student* sa = *(Student**)a;
    Student* sb = *(Student**)b;
    if (sa->totalScore > sb->totalScore) return -1;
    if (sa->totalScore < sb->totalScore) return 1;
    return 0;
}

//同班同学成绩排名
void studentQueryClassScore(Student* s) {
    printf("\n=== 本班成绩查询 ===\n");
    int count = 0;
    Student* curr = studentHead;

    //计算本班学生数
    while (curr) {
        if (strcmp(curr->className, s->className) == 0) count++;
        curr = curr->next;
    }
    if (count == 0) {
        printf("本班暂无学生\n");
        return;
    }

    Student** arr = (Student**)malloc(count * sizeof(Student*));
    int idx = 0;
    curr = studentHead;
    while (curr) {
        if (strcmp(curr->className, s->className) == 0) {
            arr[idx++] = curr;
        }
        curr = curr->next;
    }

    qsort(arr, count, sizeof(Student*), compareStudentByTotalScore);

    for (int i = 0; i < count; i++) {
        arr[i]->rank = i + 1;
    }

    printf("学号\t姓名\t总分\t排名\n");
    printf("-------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%s\t%s\t%.2lf\t%d\n", arr[i]->id, arr[i]->name, arr[i]->totalScore, arr[i]->rank);
    }
    free(arr);
}

//成绩分析
void studentScoreAnalysis(Student* s) {
    printf("\n=== 成绩分析（条形图）===\n");
    if (!s->scoreHead) {
        printf("暂无成绩记录\n");
        return;
    }
    Score* sc = s->scoreHead;
    while (sc) {
        printf("%-10s|", sc->courseName);//格式化输出，左对齐10位
        int bars = (int)(sc->score / 5);
        for (int i = 0; i < bars; i++) printf("█");
        printf(" %.2lf\n", sc->score);
        sc = sc->next;
    }
}

//修改密码
void studentChangePassword(Student* s) {
    printf("\n=== 密码修改 ===\n");
    char oldPwd[MAX_PWD], newPwd[MAX_PWD];
    printf("请输入旧密码：");
    getPassword(oldPwd);
    if (strcmp(s->password, oldPwd) != 0) {
        printf("旧密码错误！\n");
        return;
    }
    printf("请输入新密码：");
    getPassword(newPwd);
    strcpy(s->password, newPwd);
    saveStudents();
    printf("密码修改成功！\n");
}

//申诉
void studentAppeal(Student* s) {
    printf("\n=== 成绩申诉 ===\n");
    if (!s->scoreHead) {
        printf("您还没有成绩记录，无法申诉。\n");
        return;
    }
    char course[MAX_COURSE];
    double newScore;
    char reason[MAX_REASON];
    printf("请输入要申诉的课程名：");
    scanf("%s", course);
    Score* sc = s->scoreHead;
    while (sc) {
        if (strcmp(sc->courseName, course) == 0) break;
        sc = sc->next;
    }
    if (!sc) {
        printf("没有这门课程的成绩。\n");
        return;
    }
    printf("原成绩：%.2lf\n", sc->score);
    printf("请输入期望的新成绩：");
    scanf("%lf", &newScore);
    printf("请输入申诉理由：");
    clearInputBuffer();
    fgets(reason, sizeof(reason), stdin);
    reason[strcspn(reason, "\n")] = 0;//移除末尾换行符

    // 检查是否已有未处理的申诉
    if (findAppeal(s->id, course)) {
        printf("该课程已有未处理的申诉，请等待。\n");
        return;
    }

    Appeal* a = createAppeal(s->id, course, sc->score, newScore, reason);
    addAppeal(a);
    saveAppeals();
    printf("申诉已提交，等待教师或管理员处理。\n");
}

//学生登录后功能版面
void studentMenu(Student* s) {
    int choice;
    while (1) {
        printf("\n=== 学生端 ===\n");
        printf("1. 成绩查询\n");
        printf("2. 查询本班成绩\n");
        printf("3. 成绩分析\n");
        printf("4. 密码修改\n");
        printf("5. 成绩申诉\n");
        printf("0. 返回上一级\n");
        printf("请选择：");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: studentQueryScore(s); break;
        case 2: studentQueryClassScore(s); break;
        case 3: studentScoreAnalysis(s); break;
        case 4: studentChangePassword(s); break;
        case 5: studentAppeal(s); break;
        case 0: return;
        default: printf("无效选择\n");
        }
    }
}

// ==================== 教师端功能 ====================

//教师新增学生
void teacherAddStudent(Teacher* t) {
    printf("\n=== 新增学生 ===\n");
    char id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD];
    printf("学号："); scanf("%s", id);
    if (findStudent(id)) {
        printf("学号已存在\n");
        return;
    }
    printf("姓名："); scanf("%s", name);
    printf("密码："); getPassword(pwd);

    Student* s = createStudent(id, name, pwd, t->className);
    addStudent(s);
    saveStudents();
    printf("新增学生成功\n");
}

//删除学生
void teacherDeleteStudent(Teacher* t) {
    printf("\n=== 删除学生 ===\n");
    char id[MAX_ID];
    printf("学号："); scanf("%s", id);
    Student* s = findStudent(id);
    if (!s || strcmp(s->className, t->className) != 0) {
        printf("学生不存在或不属于本班\n");
        return;
    }
    deleteStudent(id);
    saveStudents();
    printf("删除学生成功\n");
}

//修改学生信息
void teacherModifyStudent(Teacher* t) {
    printf("\n=== 修改学生信息 ===\n");
    char id[MAX_ID];
    printf("学号："); scanf("%s", id);
    Student* s = findStudent(id);
    if (!s || strcmp(s->className, t->className) != 0) {
        printf("学生不存在或不属于本班\n");
        return;
    }

    char name[MAX_NAME], pwd[MAX_PWD];
    printf("新姓名（留空不修改）：");
    clearInputBuffer();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    if (strlen(name) > 0) {
        strcpy(s->name, name);
    }

    printf("新密码（留空不修改）：");
    fgets(pwd, sizeof(pwd), stdin);
    pwd[strcspn(pwd, "\n")] = 0;
    if (strlen(pwd) > 0) {
        strcpy(s->password, pwd);
    }

    saveStudents();
    printf("修改学生信息成功\n");
}

//查看本班成绩
void teacherViewClassScore(Teacher* t) {
    printf("\n=== 本班成绩查看（分页显示）===\n");
    int count = 0;
    Student* curr = studentHead;
    while (curr) {
        if (strcmp(curr->className, t->className) == 0) count++;
        curr = curr->next;
    }
    if (count == 0) {
        printf("本班暂无学生\n");
        return;
    }

    Student** arr = (Student**)malloc(count * sizeof(Student*));
    int idx = 0;
    curr = studentHead;
    while (curr) {
        if (strcmp(curr->className, t->className) == 0) {
            arr[idx++] = curr;
        }
        curr = curr->next;
    }

    qsort(arr, count, sizeof(Student*), compareStudentByTotalScore);
    for (int i = 0; i < count; i++) {
        arr[i]->rank = i + 1;
    }

    int pageSize = 5;
    int totalPages = (count + pageSize - 1) / pageSize;//向上取整
    int currentPage = 1;
    int choice;

    while (1) {
        printf("\n当前页：%d/%d，共%d名学生\n", currentPage, totalPages, count);
        printf("学号\t姓名\t总分\t排名\n");
        printf("-------------------------\n");
        int start = (currentPage - 1) * pageSize;
        int end = start + pageSize;
        if (end > count) end = count;
        for (int i = start; i < end; i++) {
            printf("%s\t%s\t%.2lf\t%d\n", arr[i]->id, arr[i]->name, arr[i]->totalScore, arr[i]->rank);
        }
        printf("-------------------------\n");
        printf("操作：1.下一页 2.上一页 3.跳转到页码 0.返回\n");
        printf("请选择：");
        scanf("%d", &choice);
        if (choice == 1) {
            if (currentPage < totalPages) currentPage++;
            else printf("已是最后一页\n");
        }
        else if (choice == 2) {
            if (currentPage > 1) currentPage--;
            else printf("已是第一页\n");
        }
        else if (choice == 3) {
            printf("请输入页码（1-%d）：", totalPages);
            int page;
            scanf("%d", &page);
            if (page >= 1 && page <= totalPages) currentPage = page;
            else printf("页码无效\n");
        }
        else if (choice == 0) {
            break;
        }
        else {
            printf("无效选择\n");
        }
    }
    free(arr);
}

//录入成绩
void teacherAddScore(Teacher* t) {
    printf("\n=== 录入成绩 ===\n");
    char id[MAX_ID], course[MAX_COURSE];
    double score;
    printf("学号："); scanf("%s", id);
    Student* s = findStudent(id);
    if (!s || strcmp(s->className, t->className) != 0) {
        printf("学生不存在或不属于本班\n");
        return;
    }
    printf("课程名："); scanf("%s", course);
    printf("成绩："); scanf("%lf", &score);

    Score* sc = s->scoreHead;
    int updated = 0;
    while (sc) {
        if (strcmp(sc->courseName, course) == 0) {
            s->totalScore -= sc->score;
            sc->score = score;
            s->totalScore += score;
            int courseCount = getCourseCount(s);
            s->avgScore = courseCount > 0 ? s->totalScore / courseCount : 0;
            saveScores();
            printf("成绩更新成功\n");
            updated = 1;
            break;
        }
        sc = sc->next;
    }

    if (!updated) {
        Score* newSc = (Score*)malloc(sizeof(Score));
        strcpy(newSc->courseName, course);
        newSc->score = score;
        newSc->next = NULL;
        newSc->prev = NULL;
        if (!s->scoreHead) s->scoreHead = newSc;
        else {
            Score* curr = s->scoreHead;
            while (curr->next) curr = curr->next;
            curr->next = newSc;
            newSc->prev = curr;
        }
        s->totalScore += score;
        int courseCount = getCourseCount(s);
        s->avgScore = courseCount > 0 ? s->totalScore / courseCount : 0;
        saveScores();
        printf("成绩录入成功\n");
    }
}

//导出学生信息
void teacherExportStudents(Teacher* t) {
    printf("\n=== 导出学生信息 ===\n");
    char filename[100000];
    printf("导出文件名："); scanf("%s", filename);
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("文件打开失败\n");
        return;
    }
    fprintf(fp, "学号,姓名,班级,总分,平均分\n");
    Student* curr = studentHead;
    while (curr) {
        if (strcmp(curr->className, t->className) == 0) {
            fprintf(fp, "%s,%s,%s,%.2lf,%.2lf\n",
                curr->id, curr->name, curr->className,
                curr->totalScore, curr->avgScore);
        }
        curr = curr->next;
    }
    fclose(fp);
    printf("导出成功\n");
}

//修改密码
void teacherChangePassword(Teacher* t) {
    printf("\n=== 密码修改 ===\n");
    char oldPwd[MAX_PWD], newPwd[MAX_PWD];
    printf("请输入旧密码：");
    getPassword(oldPwd);
    if (strcmp(t->password, oldPwd) != 0) {
        printf("旧密码错误！\n");
        return;
    }
    printf("请输入新密码：");
    getPassword(newPwd);
    strcpy(t->password, newPwd);
    saveTeachers();
    printf("密码修改成功！\n");
}

//成绩分析(平均分)
void teacherScoreAnalysis(Teacher* t) {
    printf("\n=== 班级成绩分析（各科平均分）===\n");
#define MAX_COURSES 50
    char courseNames[MAX_COURSES][MAX_COURSE];
    double courseTotal[MAX_COURSES] = { 0 };
    int courseCount[MAX_COURSES] = { 0 };
    int courseNum = 0;

    Student* curr = studentHead;
    while (curr) {
        if (strcmp(curr->className, t->className) == 0) {
            Score* sc = curr->scoreHead;
            while (sc) {
                int found = -1;
                for (int i = 0; i < courseNum; i++) {
                    if (strcmp(courseNames[i], sc->courseName) == 0) {
                        found = i;
                        break;
                    }
                }
                if (found == -1) {
                    if (courseNum >= MAX_COURSES) {
                        printf("课程数量过多，无法分析。\n");
                        return;
                    }
                    strcpy(courseNames[courseNum], sc->courseName);
                    courseTotal[courseNum] = sc->score;
                    courseCount[courseNum] = 1;
                    courseNum++;
                }
                else {
                    courseTotal[found] += sc->score;
                    courseCount[found]++;
                }
                sc = sc->next;
            }
        }
        curr = curr->next;
    }

    if (courseNum == 0) {
        printf("本班暂无成绩记录。\n");
        return;
    }

    printf("课程\t平均分\t条形图\n");
    printf("-------------------------\n");
    for (int i = 0; i < courseNum; i++) {
        double avg = courseTotal[i] / courseCount[i];
        printf("%-10s|", courseNames[i]);
        int bars = (int)(avg / 5);
        for (int j = 0; j < bars; j++) printf("█");
        printf(" %.2lf\n", avg);
    }
}

//处理申诉
void teacherViewAppeals(Teacher* t) {
    printf("\n=== 本班待处理申诉 ===\n");
    int found = 0;
    Appeal* curr = appealHead;
    while (curr) {
        if (curr->status == 0) {
            Student* s = findStudent(curr->studentId);
            if (s && strcmp(s->className, t->className) == 0) {
                found = 1;
                printf("学号：%s 课程：%s 原成绩：%.2lf 期望：%.2lf\n",
                    curr->studentId, curr->courseName, curr->oldScore, curr->newScore);
                printf("理由：%s\n", curr->reason);
                printf("操作：1.同意修改 2.拒绝（删除申诉） 0.跳过\n");
                int op;
                scanf("%d", &op);
                if (op == 1) {
                    // 修改成绩
                    Student* stu = findStudent(curr->studentId);
                    Score* sc = stu->scoreHead;
                    while (sc) {
                        if (strcmp(sc->courseName, curr->courseName) == 0) {
                            stu->totalScore -= sc->score;
                            sc->score = curr->newScore;
                            stu->totalScore += curr->newScore;
                            int courseCount = getCourseCount(stu);
                            stu->avgScore = courseCount > 0 ? stu->totalScore / courseCount : 0;
                            saveScores();
                            break;
                        }
                        sc = sc->next;
                    }
                    curr->status = 1;
                    saveAppeals();
                    printf("成绩已修改，申诉处理完成。\n");
                }
                else if (op == 2) {
                    Appeal* temp = curr;
                    curr = curr->next;
                    deleteAppeal(temp);
                    saveAppeals();
                    printf("申诉已拒绝并删除。\n");
                    continue; // 跳过curr=curr->next
                }
            }
        }
        curr = curr->next;
    }
    if (!found) printf("暂无待处理申诉。\n");
}

//教师功能面板
void teacherMenu(Teacher* t) {
    int choice;
    while (1) {
        printf("\n=== 教师端 ===\n");
        printf("1. 增删改查学生信息\n");
        printf("   1) 新增学生\n");
        printf("   2) 删除学生\n");
        printf("   3) 修改学生信息\n");
        printf("2. 查看班内成绩（分页）\n");
        printf("3. 录入成绩\n");
        printf("4. 学生信息下载至文件\n");
        printf("5. 成绩分析\n");
        printf("6. 密码修改\n");
        printf("7. 查看待办（成绩申诉）\n");
        printf("0. 返回上一层\n");
        printf("请选择：");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: {
            int sub;
            printf("请选择子操作：");
            scanf("%d", &sub);
            if (sub == 1) teacherAddStudent(t);
            else if (sub == 2) teacherDeleteStudent(t);
            else if (sub == 3) teacherModifyStudent(t);
            else printf("无效选择\n");
            break;
        }
        case 2: teacherViewClassScore(t); break;
        case 3: teacherAddScore(t); break;
        case 4: teacherExportStudents(t); break;
        case 5: teacherScoreAnalysis(t); break;
        case 6: teacherChangePassword(t); break;
        case 7: teacherViewAppeals(t); break;
        case 0: return;
        default: printf("无效选择\n");
        }
    }
}

// ==================== 管理员端功能 ====================

//管理员注册学生/教师账户
void adminRegisterAccount() {
    printf("\n=== 账号注册（管理员端）===\n");
    char type[10], id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD], className[MAX_CLASS];
    printf("账号类型（student/teacher）："); scanf("%s", type);
    if (strcmp(type, "student") != 0 && strcmp(type, "teacher") != 0) {
        printf("无效类型\n");
        return;
    }
    printf("账号ID："); scanf("%s", id);
    if (strcmp(type, "student") == 0 && findStudent(id)) {
        printf("学号已存在\n");
        return;
    }
    if (strcmp(type, "teacher") == 0 && findTeacher(id)) {
        printf("教师号已存在\n");
        return;
    }
    printf("姓名："); scanf("%s", name);
    printf("密码："); getPassword(pwd);
    if (strcmp(type, "student") == 0) {
        printf("班级："); scanf("%s", className);
        Student* s = createStudent(id, name, pwd, className);
        addStudent(s);
        saveStudents();
    }
    else {
        printf("班级："); scanf("%s", className);
        Teacher* t = createTeacher(id, name, pwd, className);
        addTeacher(t);
        saveTeachers();
    }
    printf("注册成功\n");
}

//找回密码
void adminFindPassword() {
    printf("\n=== 密码找回 ===\n");
    char type[10], id[MAX_ID], name[MAX_NAME];
    printf("账号类型（student/teacher/admin）："); scanf("%s", type);
    printf("账号ID："); scanf("%s", id);
    if (strcmp(type, "student") == 0 || strcmp(type, "teacher") == 0) {
        printf("姓名："); scanf("%s", name);
    }

    if (strcmp(type, "student") == 0) {
        Student* s = findStudent(id);
        if (s && strcmp(s->name, name) == 0) {
            printf("密码：%s\n", s->password);
        }
        else {
            printf("信息不匹配\n");
        }
    }
    else if (strcmp(type, "teacher") == 0) {
        Teacher* t = findTeacher(id);
        if (t && strcmp(t->name, name) == 0) {
            printf("密码：%s\n", t->password);
        }
        else {
            printf("信息不匹配\n");
        }
    }
    else if (strcmp(type, "admin") == 0) {
        Admin* a = findAdmin(id);
        if (a) {
            printf("密码：%s\n", a->password);
        }
        else {
            printf("信息不匹配\n");
        }
    }
    else {
        printf("无效类型\n");
    }
}

//处理申诉
void adminScoreAppeal() {
    printf("\n=== 成绩申诉处理 ===\n");
    // 显示所有待处理申诉
    int found = 0;
    Appeal* curr = appealHead;
    while (curr) {
        if (curr->status == 0) {
            found = 1;
            Student* s = findStudent(curr->studentId);
            printf("学号：%s 姓名：%s 班级：%s\n", curr->studentId, s ? s->name : "未知", s ? s->className : "未知");
            printf("课程：%s 原成绩：%.2lf 期望：%.2lf\n", curr->courseName, curr->oldScore, curr->newScore);
            printf("理由：%s\n", curr->reason);
            printf("操作：1.同意修改 2.拒绝（删除申诉） 0.跳过\n");
            int op;
            scanf("%d", &op);
            if (op == 1) {
                Student* stu = findStudent(curr->studentId);
                if (stu) {
                    Score* sc = stu->scoreHead;
                    while (sc) {
                        if (strcmp(sc->courseName, curr->courseName) == 0) {
                            stu->totalScore -= sc->score;
                            sc->score = curr->newScore;
                            stu->totalScore += curr->newScore;
                            int courseCount = getCourseCount(stu);
                            stu->avgScore = courseCount > 0 ? stu->totalScore / courseCount : 0;
                            saveScores();
                            break;
                        }
                        sc = sc->next;
                    }
                }
                curr->status = 1;
                saveAppeals();
                printf("成绩已修改，申诉处理完成。\n");
            }
            else if (op == 2) {
                Appeal* temp = curr;
                curr = curr->next;
                deleteAppeal(temp);
                saveAppeals();
                printf("申诉已拒绝并删除。\n");
                continue;
            }
        }
        curr = curr->next;
    }
    if (!found) printf("暂无待处理申诉。\n");
}

//修改密码(所有用户)
void adminModifyPassword() {
    printf("\n=== 密码修改 ===\n");
    char type[10], id[MAX_ID], oldPwd[MAX_PWD], newPwd[MAX_PWD];
    printf("账号类型（student/teacher/admin）："); scanf("%s", type);
    printf("账号ID："); scanf("%s", id);
    printf("旧密码："); getPassword(oldPwd);

    if (strcmp(type, "student") == 0) {
        Student* s = findStudent(id);
        if (s && strcmp(s->password, oldPwd) == 0) {
            printf("新密码："); getPassword(newPwd);
            strcpy(s->password, newPwd);
            saveStudents();
            printf("修改成功\n");
        }
        else {
            printf("旧密码错误\n");
        }
    }
    else if (strcmp(type, "teacher") == 0) {
        Teacher* t = findTeacher(id);
        if (t && strcmp(t->password, oldPwd) == 0) {
            printf("新密码："); getPassword(newPwd);
            strcpy(t->password, newPwd);
            saveTeachers();
            printf("修改成功\n");
        }
        else {
            printf("旧密码错误\n");
        }
    }
    else if (strcmp(type, "admin") == 0) {
        Admin* a = findAdmin(id);
        if (a && strcmp(a->password, oldPwd) == 0) {
            printf("新密码："); getPassword(newPwd);
            strcpy(a->password, newPwd);
            saveAdmins();
            printf("修改成功\n");
        }
        else {
            printf("旧密码错误\n");
        }
    }
    else {
        printf("无效类型\n");
    }
}

//管理用户
void adminCRUD() {
    printf("\n=== 增删改查账号及学生信息 ===\n");
    int choice;
    printf("1. 新增账号\n");
    printf("2. 删除账号\n");
    printf("3. 修改账号信息\n");
    printf("请选择：");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        adminRegisterAccount();
    }
    else if (choice == 2) {
        char type[10], id[MAX_ID];
        printf("账号类型（student/teacher/admin）："); scanf("%s", type);
        printf("账号ID："); scanf("%s", id);

        if (strcmp(type, "student") == 0) {
            deleteStudent(id);
            saveStudents();
            printf("删除成功\n");
        }
        else if (strcmp(type, "teacher") == 0) {
            deleteTeacher(id);
            saveTeachers();
            printf("删除成功\n");
        }
        else if (strcmp(type, "admin") == 0) {
            deleteAdmin(id);
            saveAdmins();
            printf("删除成功\n");
        }
        else {
            printf("无效类型\n");
        }
    }
    else if (choice == 3) {
        char type[10], id[MAX_ID], name[MAX_NAME], className[MAX_CLASS];
        printf("账号类型（student/teacher）："); scanf("%s", type);
        printf("账号ID："); scanf("%s", id);

        if (strcmp(type, "student") == 0) {
            Student* s = findStudent(id);
            if (s) {
                printf("新姓名（留空不修改）：");
                clearInputBuffer();
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                if (strlen(name) > 0) strcpy(s->name, name);

                printf("新班级（留空不修改）：");
                fgets(className, sizeof(className), stdin);
                className[strcspn(className, "\n")] = 0;
                if (strlen(className) > 0) strcpy(s->className, className);

                saveStudents();
                printf("修改成功\n");
            }
            else {
                printf("账号不存在\n");
            }
        }
        else if (strcmp(type, "teacher") == 0) {
            Teacher* t = findTeacher(id);
            if (t) {
                printf("新姓名（留空不修改）：");
                clearInputBuffer();
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                if (strlen(name) > 0) strcpy(t->name, name);

                printf("新班级（留空不修改）：");
                fgets(className, sizeof(className), stdin);
                className[strcspn(className, "\n")] = 0;
                if (strlen(className) > 0) strcpy(t->className, className);

                saveTeachers();
                printf("修改成功\n");
            }
            else {
                printf("账号不存在\n");
            }
        }
        else {
            printf("无效类型\n");
        }
    }
    else {
        printf("无效选择\n");
    }
}

void adminImportFromFile() {
    printf("\n=== 从文件录入账号密码 ===\n");
    char filename[100];
    printf("文件名："); scanf("%s", filename);
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("文件打开失败\n");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char type[16], id[MAX_ID], name[MAX_NAME], pwd[MAX_PWD], className[MAX_CLASS];
        if (sscanf(line, "%s %s %s %s %s", type, id, name, pwd, className) != 5) continue;

        if (strcmp(type, "student") == 0) {
            if (!findStudent(id)) {
                Student* s = createStudent(id, name, pwd, className);
                addStudent(s);
            }
        }
        else if (strcmp(type, "teacher") == 0) {
            if (!findTeacher(id)) {
                Teacher* t = createTeacher(id, name, pwd, className);
                addTeacher(t);
            }
        }
    }
    fclose(fp);
    saveStudents();
    saveTeachers();
    printf("录入成功\n");
}

//导出所有账户信息到文件
void adminExportToFile() {
    printf("\n=== 导出所有账号密码到文件 ===\n");
    char filename[100];
    printf("文件名："); scanf("%s", filename);
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("文件打开失败\n");
        return;
    }

    fprintf(fp, "账号类型\t账号ID\t姓名\t密码\t班级\n");
    Student* s = studentHead;
    while (s) {
        fprintf(fp, "student\t%s\t%s\t%s\t%s\n", s->id, s->name, s->password, s->className);
        s = s->next;
    }
    Teacher* t = teacherHead;
    while (t) {
        fprintf(fp, "teacher\t%s\t%s\t%s\t%s\n", t->id, t->name, t->password, t->className);
        t = t->next;
    }
    Admin* a = adminHead;
    while (a) {
        fprintf(fp, "admin\t%s\t-\t%s\t-\n", a->id, a->password);
        a = a->next;
    }
    fclose(fp);
    printf("导出成功\n");
}

//管理员登录教师端
void adminLoginTeacher() {
    printf("\n=== 登录教师端 ===\n");
    char id[MAX_ID], pwd[MAX_PWD];
    printf("教师号："); scanf("%s", id);
    printf("密码："); getPassword(pwd);
    Teacher* t = findTeacher(id);
    if (t && strcmp(t->password, pwd) == 0) {
        teacherMenu(t);
    }
    else {
        printf("账号或密码错误\n");
    }
}

//管理员功能面板
void adminMenu() {
    int choice;
    while (1) {
        printf("\n=== 管理员端 ===\n");
        printf("1. 账号注册\n");
        printf("2. 密码找回\n");
        printf("3. 成绩申诉\n");
        printf("4. 密码修改\n");
        printf("5. 增删改查账号及学生信息\n");
        printf("6. 从文件录入所有账号密码\n");
        printf("7. 从文件导出所有账号密码\n");
        printf("8. 登录教师端进行信息更改\n");
        printf("0. 返回上一层\n");
        printf("请选择：");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: adminRegisterAccount(); break;
        case 2: adminFindPassword(); break;
        case 3: adminScoreAppeal(); break;
        case 4: adminModifyPassword(); break;
        case 5: adminCRUD(); break;
        case 6: adminImportFromFile(); break;
        case 7: adminExportToFile(); break;
        case 8: adminLoginTeacher(); break;
        case 0: return;
        default: printf("无效选择\n");
        }
    }
}

// ==================== 登录界面 ====================
void studentLogin() {
    printf("\n=== 学生登录 ===\n");
    char id[MAX_ID], pwd[MAX_PWD];
    printf("学号："); scanf("%s", id);
    printf("密码："); getPassword(pwd);
    Student* s = findStudent(id);
    if (s && strcmp(s->password, pwd) == 0) {
        studentMenu(s);
    }
    else {
        printf("账号或密码错误\n");
    }
}

void teacherLogin() {
    printf("\n=== 教师登录 ===\n");
    char id[MAX_ID], pwd[MAX_PWD];
    printf("教师号："); scanf("%s", id);
    printf("密码："); getPassword(pwd);
    Teacher* t = findTeacher(id);
    if (t && strcmp(t->password, pwd) == 0) {
        teacherMenu(t);
    }
    else {
        printf("账号或密码错误\n");
    }
}

void adminLogin() {
    printf("\n=== 管理员登录 ===\n");
    char id[MAX_ID], pwd[MAX_PWD];
    printf("管理员ID："); scanf("%s", id);
    printf("密码："); getPassword(pwd);
    Admin* a = findAdmin(id);
    if (a && strcmp(a->password, pwd) == 0) {
        adminMenu();
    }
    else {
        printf("账号或密码错误\n");
    }
}

// ==================== 主函数 ====================
int main() {
    // 加载已有数据
    loadAdmins();
    loadTeachers();
    loadStudents();
    loadAppeals();

    while (1) {
        printf("\n======= 学生信息管理系统 =======\n");
        printf("          1. 账号注册          \n");
        printf("          2. 学生登录          \n");
        printf("          3. 教师登录          \n");
        printf("          4. 管理员登录        \n");
        printf("          5. 密码找回          \n");
        printf("          0. 退出系统          \n");
        printf("================================\n");
        printf("请选择功能：");
        int choice;
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: generalRegister(); break;
        case 2: studentLogin(); break;
        case 3: teacherLogin(); break;
        case 4: adminLogin(); break;
        case 5: passwordRecovery(); break;
        case 0:
            // 释放内存
        {
            Student* s = studentHead;
            while (s) {
                Student* tempS = s;
                s = s->next;
                Score* sc = tempS->scoreHead;
                while (sc) {
                    Score* tempSc = sc;
                    sc = sc->next;
                    free(tempSc);
                }
                free(tempS);
            }
            Teacher* t = teacherHead;
            while (t) {
                Teacher* tempT = t;
                t = t->next;
                free(tempT);
            }
            Admin* a = adminHead;
            while (a) {
                Admin* tempA = a;
                a = a->next;
                free(tempA);
            }
            Appeal* ap = appealHead;
            while (ap) {
                Appeal* tempAp = ap;
                ap = ap->next;
                free(tempAp);
            }
        }
        printf("\n系统已安全退出！\n");
        return 0;
        default:
            printf("无效选择，请输入0-5之间的数字！\n");
        }
    }
    return 0;
}
