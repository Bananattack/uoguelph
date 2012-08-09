/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#include <jni.h>
#include <stdio.h>
#include "ConfigManager.h"
#include "ParameterManager.h"

ParameterManager* GetJavaPMPointer(JNIEnv* env, jobject obj)
{
	jclass cls = (*env)->GetObjectClass(env, obj);
	jfieldID id = (*env)->GetFieldID(env, cls, "internalPointer", "I");
	return (ParameterManager*) ((*env)->GetIntField(env, obj, id));
}

void SetJavaPMPointer(JNIEnv* env, jobject obj, ParameterManager* value)
{
	jclass cls = (*env)->GetObjectClass(env, obj);
	jfieldID id = (*env)->GetFieldID(env, cls, "internalPointer", "I");
	(*env)->SetIntField(env, obj, id, (jint) value);
}

JNIEXPORT void JNICALL Java_ConfigManager_create(JNIEnv *env, jobject obj)
{
	SetJavaPMPointer(env, obj, PM_create(25));
}

JNIEXPORT void JNICALL Java_ConfigManager_destroy(JNIEnv *env, jobject obj)
{
	ParameterManager* pm = GetJavaPMPointer(env, obj);
	if (pm != NULL)
	{
		PM_destroy(pm);
		SetJavaPMPointer(env, obj, NULL);
	}
}

JNIEXPORT void JNICALL Java_ConfigManager_manage(JNIEnv *env, jobject obj, jstring parameterName, jbyte parameterType, jboolean required)
{

	ParameterManager* pm = GetJavaPMPointer(env, obj);
	const jbyte* str;
	char* name;
	char type = (char) parameterType;
	param_t ptype;
	
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		if(type == 'i')
		{
			ptype = INT_TYPE;
		}
		else if(type == 's')
		{
			ptype = STRING_TYPE;
		}
		else if(type == 'l')
		{
			ptype = LIST_TYPE;
		}
		else if(type == 'r')
		{
			ptype = REAL_TYPE;
		}
		else if(type == 'b')
		{
			ptype = BOOLEAN_TYPE;
		}
		PM_manage(pm, name, ptype, required);
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
}

JNIEXPORT jboolean JNICALL Java_ConfigManager_parseFrom(JNIEnv *env, jobject obj, jstring filename)
{
	FILE* f;
	const jbyte* str;
	jboolean result = false;
	jclass cls;
	
	str = (*env)->GetStringUTFChars(env, filename, NULL);
	if (str != NULL)
	{
		f = fopen(str, "r");
		result = PM_parseFrom(GetJavaPMPointer(env, obj), f, '#');
		(*env)->ReleaseStringUTFChars(env, filename, str);
		if(f)
		{
			fclose(f);
		}
	}
	if(!result)
	{
		cls = (*env)->FindClass(env, "ConfigParseException");
		(*env)->ThrowNew(env, cls, PM_error_message);
	}
	return result;
}

JNIEXPORT jboolean JNICALL Java_ConfigManager_hasValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	jboolean result = false;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		result = PM_hasValue(GetJavaPMPointer(env, obj), name);
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return result;
}

JNIEXPORT jstring JNICALL Java_ConfigManager_getStringValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	char* result = NULL;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		result = PM_getValue(GetJavaPMPointer(env, obj), name).str_val;
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return (*env)->NewStringUTF(env, result);
}

JNIEXPORT jint JNICALL Java_ConfigManager_getIntValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	int result = 0;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		result = PM_getValue(GetJavaPMPointer(env, obj), name).int_val;
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return result;
}

JNIEXPORT jfloat JNICALL Java_ConfigManager_getFloatValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	float result = 0.0;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		result = PM_getValue(GetJavaPMPointer(env, obj), name).real_val;
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return result;
}

JNIEXPORT jboolean JNICALL Java_ConfigManager_getBooleanValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	jboolean result = false;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		result = PM_getValue(GetJavaPMPointer(env, obj), name).bool_val;
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return result;
}

JNIEXPORT jobjectArray JNICALL Java_ConfigManager_getListValue(JNIEnv * env, jobject obj, jstring parameterName)
{
	const jbyte* str;
	char* name;
	char* value;
	int i;
	ParameterList* list;
	jobjectArray result = NULL;
	str = (*env)->GetStringUTFChars(env, parameterName, NULL);
	if (str != NULL)
	{
		name = StringNew(str);
		list = PM_getValue(GetJavaPMPointer(env, obj), name).list_val;
		result = (jobjectArray) (*env)->NewObjectArray(env, PL_getLength(list), (*env)->FindClass(env, "java/lang/String"), NULL);
		i = 0;
		while((value = PL_next(list)) != NULL)
		{
			(*env)->SetObjectArrayElement(env, result, i, (*env)->NewStringUTF(env, value));
			i++;
		}
		(*env)->ReleaseStringUTFChars(env, parameterName, str);
		StringFree(name);
	}
	return result;
}
