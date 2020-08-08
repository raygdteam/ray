// Любезно взято из o2
#include "syntax_tree.hpp"

#include <algorithm>

SyntaxFile::SyntaxFile() :
	mGlobalNamespace(new SyntaxNamespace())
{}

SyntaxFile::~SyntaxFile()
{
	delete mGlobalNamespace;
}

const string& SyntaxFile::GetPath() const
{
	return mPath;
}

const string& SyntaxFile::GetData() const
{
	return mData;
}

const TimeStamp& SyntaxFile::GetLastEditedDate() const
{
	return mLastEditedDate;
}

SyntaxNamespace* SyntaxFile::GetGlobalNamespace() const
{
	return mGlobalNamespace;
}

int ISyntaxExpression::GetBegin() const
{
	return mBegin;
}

int ISyntaxExpression::GetLength() const
{
	return mLength;
}

int ISyntaxExpression::GetEnd() const
{
	return mBegin + mLength;
}

int ISyntaxExpression::GetLine() const
{
	return mLine;
}

const string& ISyntaxExpression::GetData() const
{
	return mData;
}

SyntaxFile* ISyntaxExpression::GetOwnerFile() const
{
	return mFile;
}

SyntaxSection::SyntaxSection()
{}

SyntaxSection::~SyntaxSection()
{
	for (auto x : mFunctions)
		delete x;

	for (auto x : mVariables)
		delete x;

	for (auto x : mEnums)
		delete x;

	for (auto x : mSections)
		delete x;

	for (auto x : mComments)
		delete x;

	for (auto x : mTypedefs)
		delete x;

	for (auto x : mUsingNamespaces)
		delete x;
}

SyntaxSection* SyntaxSection::GetParentSection() const
{
	return mParentSection;
}

const string& SyntaxSection::GetName() const
{
	return mName;
}

const string& SyntaxSection::GetFullName() const
{
	return mFullName;
}

const SyntaxFunctionsVec& SyntaxSection::GetFunctions() const
{
	return mFunctions;
}

const SyntaxVariablesVec& SyntaxSection::GetVariables() const
{
	return mVariables;
}

const SyntaxSectionsVec& SyntaxSection::GetSections() const
{
	return mSections;
}

const SyntaxEnumsVec& SyntaxSection::GetEnums() const
{
	return mEnums;
}

const SyntaxTypedefsVec& SyntaxSection::GetTypedefs() const
{
	return mTypedefs;
}

const SyntaxUsingNamespacesVec& SyntaxSection::GetUsingNamespaces() const
{
	return mUsingNamespaces;
}

const SyntaxCommentsVec& SyntaxSection::GetComments() const
{
	return mComments;
}

SyntaxComment* SyntaxSection::FindCommentNearLine(int line) const
{
	SyntaxComment* synComment = nullptr;
	for (auto comment : mComments)
	{
		// comment is on same line as variable
		if (comment->GetLine() == line)
		{
			synComment = comment;
			break;
		}

		// comment is on up line to variable
		if (comment->GetLine() == line - 1)
		{
			// check other variable on this line
			bool success = true;
			for (auto v : mVariables)
			{
				if (v->GetLine() == comment->GetLine())
				{
					success = false;
					break;
				}
			}

			if (success)
			{
				synComment = comment;
				break;
			}
		}
	}

	return synComment;
}

SyntaxSectionsVec SyntaxSection::GetAllSections() const
{
	SyntaxSectionsVec res = mSections;

	for (auto x : mSections)
	{
		for (auto y : x->GetAllSections())
			res.push_back(y);
	}

	return res;
}

SyntaxClassesVec SyntaxSection::GetAllClasses() const
{
	SyntaxClassesVec res;

	for (auto x : mSections)
	{
		if (x->IsClass())
			res.push_back(dynamic_cast<SyntaxClass*>(x));
	}

	for (auto x : mSections)
	{
		for (auto y : x->GetAllClasses())
			res.push_back(y);
	}

	return res;
}

SyntaxEnumsVec SyntaxSection::GetAllEnums() const
{
	SyntaxEnumsVec res = mEnums;

	for (auto x : mSections)
	{
		for (auto y : x->GetAllEnums())
			res.push_back(y);
	}

	return res;
}

bool SyntaxSection::IsClass() const
{
	return false;
}

const SyntaxAttributesVec& SyntaxSection::GetAttributes() const
{
	return mAttributes;
}

SyntaxNamespace::SyntaxNamespace()
{}

const SyntaxFunctionsVec& SyntaxClass::GetFunctions() const
{
	if (mSourceClass)
		return mSourceClass->GetFunctions();

	return SyntaxSection::GetFunctions();
}

const SyntaxVariablesVec& SyntaxClass::GetVariables() const
{
	if (mSourceClass)
		return mSourceClass->GetVariables();

	return SyntaxSection::GetVariables();
}

const SyntaxSectionsVec& SyntaxClass::GetSections() const
{
	if (mSourceClass)
		return mSourceClass->GetSections();

	return SyntaxSection::GetSections();
}

const SyntaxEnumsVec& SyntaxClass::GetEnums() const
{
	if (mSourceClass)
		return mSourceClass->GetEnums();

	return SyntaxSection::GetEnums();
}

const SyntaxTypedefsVec& SyntaxClass::GetTypedefs() const
{
	if (mSourceClass)
		return mSourceClass->GetTypedefs();

	return SyntaxSection::GetTypedefs();
}

const SyntaxUsingNamespacesVec& SyntaxClass::GetUsingNamespaces() const
{
	if (mSourceClass)
		return mSourceClass->GetUsingNamespaces();

	return SyntaxSection::GetUsingNamespaces();
}

SyntaxSectionsVec SyntaxClass::GetAllSections() const
{
	if (mSourceClass)
		return mSourceClass->GetAllSections();

	return SyntaxSection::GetAllSections();
}

SyntaxClassesVec SyntaxClass::GetAllClasses() const
{
	if (mSourceClass)
		return mSourceClass->GetAllClasses();

	return SyntaxSection::GetAllClasses();
}

bool SyntaxClass::IsClass() const
{
	return true;
}

const SyntaxClassInheritancsVec& SyntaxClass::GetBaseClasses() const
{
	return mBaseClasses;
}

bool SyntaxClass::IsTemplate() const
{
	if (!mTemplateParameters.empty())
		return true;

	if (mParentSection)
	{
		SyntaxClass* cls = dynamic_cast<SyntaxClass*>(mParentSection);
		return cls && cls->IsTemplate();
	}

	return false;
}

bool SyntaxClass::IsMetaClass() const
{
	return mIsMeta;
}

const string& SyntaxClass::GetTemplateParameters() const
{
	return mTemplateParameters;
}

SyntaxProtectionSection SyntaxClass::GetClassSection() const
{
	return mClassSection;
}

const string& SyntaxClass::GetAttributeCommentDef() const
{
	return mAttributeCommentDef;
}

const string& SyntaxClass::GetAttributeShortDef() const
{
	return mAttributeShortDef;
}

const SyntaxAttributesVec& SyntaxClass::GetAttributes() const
{
	if (mSourceClass)
		return mSourceClass->GetAttributes();

	return SyntaxSection::GetAttributes();
}

const string& SyntaxType::GetName() const
{
	return mName;
}

bool SyntaxType::IsConstant() const
{
	return mIsContant;
}

bool SyntaxType::IsReference() const
{
	return mIsReference;
}

bool SyntaxType::IsPointer() const
{
	return mIsPointer;
}

const SyntaxType& SyntaxVariable::GetVariableType() const
{
	return mType;
}

const string& SyntaxVariable::GetName() const
{
	return mName;
}

const std::string& SyntaxVariable::GetDefaultValue() const
{
	return mDefaultValue;
}

SyntaxProtectionSection SyntaxVariable::GetClassSection() const
{
	return mClassSection;
}

bool SyntaxVariable::IsStatic() const
{
	return mIsStatic;
}

SyntaxFunction::SyntaxFunction()
{}

SyntaxFunction::~SyntaxFunction()
{
	for (auto x : mParameters)
		delete x;
}

const SyntaxType& SyntaxFunction::GetReturnType() const
{
	return mReturnType;
}

const string& SyntaxFunction::GetName() const
{
	return mName;
}

const SyntaxVariablesVec& SyntaxFunction::GetParameters() const
{
	return mParameters;
}

SyntaxProtectionSection SyntaxFunction::GetClassSection() const
{
	return mClassSection;
}

bool SyntaxFunction::IsTemplate() const
{
	return !mTemplates.empty();
}

const string& SyntaxFunction::GetTemplates() const
{
	return mTemplates;
}

bool SyntaxFunction::IsStatic() const
{
	return mIsStatic;
}

const string& SyntaxEnum::GetName() const
{
	return mName;
}

const string& SyntaxEnum::GetFullName() const
{
	return mFullName;
}

const StringStringDict& SyntaxEnum::GetEntries() const
{
	return mEntries;
}

SyntaxProtectionSection SyntaxEnum::GetClassSection() const
{
	return mClassSection;
}

SyntaxSection* SyntaxEnum::GetOwnerSyntaxSection() const
{
	return mOwnerSection;
}

SyntaxClassInheritance::SyntaxClassInheritance(const string& className, SyntaxProtectionSection type) :
	mClassName(className), mInheritanceType(type)
{}

const string& SyntaxClassInheritance::GetClassName() const
{
	return mClassName;
}

SyntaxClass* SyntaxClassInheritance::GetClass()
{
	return mClass;
}

SyntaxProtectionSection SyntaxClassInheritance::GetInheritanceType() const
{
	return mInheritanceType;
}

bool SyntaxClassInheritance::operator==(const SyntaxClassInheritance& other) const
{
	return mInheritanceType == other.mInheritanceType && mClassName == other.mClassName;
}

const string& SyntaxUsingNamespace::GetUsingNamespaceName() const
{
	return mUsingNamespaceName;
}

SyntaxSection* SyntaxUsingNamespace::GetUsingNamespace() const
{
	return mUsingNamespace;
}

const string& SyntaxTypedef::GetWhatName() const
{
	return mWhatName;
}

const string& SyntaxTypedef::GetNewDefName() const
{
	return mNewDefName;
}

SyntaxSection* SyntaxTypedef::GetWhat() const
{
	return mWhatSection;
}

SyntaxSection* SyntaxTypedef::GetNewDef() const
{
	return mWhatSection;
}

TimeStamp::TimeStamp(int seconds /*= 0*/, int minutes /*= 0*/, int hours /*= 0*/, int days /*= 0*/, int months /*= 0*/,
	int years /*= 0*/) :
	second(seconds), minute(minutes), hour(hours), day(days), month(months), year(years)
{}

bool TimeStamp::operator!=(const TimeStamp& wt) const
{
	return !(*this == wt);
}

bool TimeStamp::operator==(const TimeStamp& wt) const
{
	return second == wt.second && minute == wt.minute && hour == wt.hour && day == wt.day && month == wt.month &&
		year == wt.year;
}

const vector<string>& SyntaxAttributes::GetAttributesList() const
{
	return mAttributesList;
}
