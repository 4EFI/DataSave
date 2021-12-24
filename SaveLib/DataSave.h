#ifndef DATASAVE
#define DATASAVE

#include <stdio.h>
#include <string.h>
#include <typeinfo>

//-----------------------------------------------------------------------------

class Data
{
    private:
        enum Types {INT, DOUBLE, CHAR};
        int  lengthTypes[3] = {20 /*INT_SIZE*/, 45 /*DOUBLE_SIZE*/, 1 /*CHAR_SIZE*/};

        char path[100];

        int GetFileSize       (FILE *file);
        int WordPositionInFile(FILE *file, const char word[]);

        template <typename type>
        int GetType(type  value);

        void SaveLogic(FILE *file, const char key[], bool isArr, int sizeArr = 0);

    public:
        Data(const char _path[] = "dataSave.dat")
        {
            strcpy(path, _path);
        }

        template <typename type>
        void Save(const char key[], type value);

        template <typename type>
        void Save(const char key[], const type arrValues[], int arrSize);

        template <typename type>
        void Load(const char key[], type *value);

        template <typename type>
        void Load(const char key[], type arrValue[], int arrSize);
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//{     Функции сохранения данных
//-----------------------------------------------------------------------------

//Сохранение переменных
template <typename type>
void Data::Save(const char key[], type value)
{
    //Если файла не существует
    FILE *file = fopen(path, "ab");
    fclose(file);

    file = fopen(path, "r+b");

    int typeValue = GetType(value);

    SaveLogic(file, key, false);

    //Записывам в файл переменную в бинарном формате
    fwrite(&value, sizeof(type), 1, file);

    fclose(file);
}

//-----------------------------------------------------------------------------

//Сохранение масивов
template <typename type>
void Data::Save(const char key[], const type arrValues[], int arrSize)
{
    //Если файла не существует
    FILE *file = fopen(path, "ab");
    fclose(file);

    file = fopen(path, "r+b");

    int typeValue = GetType(arrValues[0]);

    SaveLogic(file, key, true, arrSize);

    //Записывам в файл переменную в бинарном формате
    fwrite(arrValues, sizeof(type), arrSize, file);

    fclose(file);
}

//-----------------------------------------------------------------------------

void Data::SaveLogic(FILE *file, const char key[], bool isArr, int sizeArr)
{
    int wordPos = WordPositionInFile(file, key);

    if(wordPos == -1)
    {
        if(GetFileSize(file) > 0)
        {
            fputc('\n', file);
        }

        fseek(file, 0, SEEK_END);

        fputs(key, file);

        if(isArr)
        {
            fwrite(&sizeArr, sizeof(int), 1, file);
        }
    }
    else
    {
        int pos = 1;

        if(isArr)
        {
            pos = 5;
        }

        //Перемещаемся в позицию после ключа и типа (ключ тип "количество элементов" переменная)
        fseek(file, wordPos + pos, SEEK_SET);
    }
}

//} Конец функций сохранения
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//{     Функции взятия данных
//-----------------------------------------------------------------------------

//Загрузка переменной
template <typename type>
void Data::Load(const char key[], type *value)
{
    FILE *file = fopen(path, "rb");

    int wordPos = WordPositionInFile(file, key);

    if(wordPos == -1 || file == NULL)
    {
        return;
    }

    fseek(file, wordPos + 1, SEEK_SET);

    fread(value, sizeof(type), 1, file);

    fclose(file);
}

//Загрузка массива
template <typename type>
void Data::Load(const char key[], type arrValue[], int arrSize)
{
    FILE *file = fopen(path, "rb");

    int wordPos = WordPositionInFile(file, key);

    if(wordPos == -1 || file == NULL)
    {
        return;
    }

    fseek(file, wordPos + 1, SEEK_SET);

    int arrSizeMax = 0;
    fread(&arrSizeMax, sizeof(int), 1, file);

    if(arrSize > arrSizeMax) arrSize = arrSizeMax;

    fread(arrValue, sizeof(type), arrSize, file);

    fclose(file);
}

//}     Конец функций загрузки
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//Количество символов в файле
int Data::GetFileSize(FILE *file)
{
    int fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);

    return fileSize;
}

//-----------------------------------------------------------------------------

//Возращает позиции последней буквы слова в файле
int Data::WordPositionInFile(FILE *file, const char word[])
{
    fseek(file, 0, SEEK_SET);

    int positionWord = -1;

    int nowPositionWord = 0;
    int i = 0;
    while(true)
    {
        int ch = getc(file);

        if(ch == EOF)
        {
            break;
        }

        if(word[nowPositionWord] == ch)
        {
            nowPositionWord++;
        }
        else
        {
            nowPositionWord = 0;
        }

        if(nowPositionWord == strlen(word))
        {
            positionWord = i;
            break;
        }

        i++;
    }

    return positionWord;
}

//-----------------------------------------------------------------------------

template <typename type>
int Data::GetType(type value)
{
    int typeValue = -1;

    if     (typeid(int)    == typeid(value))
    {
        typeValue = INT;
    }
    else if(typeid(float)  == typeid(value) ||
            typeid(double) == typeid(value))
    {
        typeValue = DOUBLE;
    }
    if     (typeid(char)   == typeid(value))
    {
        typeValue = CHAR;
    }

    return typeValue;
}

//-----------------------------------------------------------------------------

#endif
