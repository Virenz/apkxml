/*
 * Copyright 2008 Android4ME
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Dmitry Skiba
 * @author change weirui
 */
#include <string>

class AttributeSet {
	virtual int getAttributeCount();
	virtual std::string getAttributeName(int index);
	virtual	std::string getAttributeValue(int index);
	virtual std::string getPositionDescription();
	virtual int getAttributeNameResource(int index);
	virtual int getAttributeListValue(int index,std::string options[],int defaultValue);
	virtual bool getAttributeBooleanValue(int index,bool defaultValue);
	virtual int getAttributeResourceValue(int index,int defaultValue);
	virtual int getAttributeIntValue(int index,int defaultValue);
	virtual int getAttributeUnsignedIntValue(int index,int defaultValue);
	virtual float getAttributeFloatValue(int index,float defaultValue);
	virtual std::string getIdAttribute();
	virtual std::string getClassAttribute();
	virtual int getIdAttributeResourceValue(int index);
	virtual int getStyleAttribute();
	virtual std::string getAttributeValue(std::string _namespace, std::string attribute);
	virtual int getAttributeListValue(std::string _namespace,std::string attribute,std::string options[],int defaultValue);
	virtual bool getAttributeBooleanValue(std::string _namespace, std::string attribute, bool defaultValue);
	virtual int getAttributeResourceValue(std::string _namespace, std::string attribute, int defaultValue);
	virtual int getAttributeIntValue(std::string _namespace, std::string attribute, int defaultValue);
	virtual int getAttributeUnsignedIntValue(std::string _namespace, std::string attribute, int defaultValue);
	virtual float getAttributeFloatValue(std::string _namespace, std::string attribute, float defaultValue);

    //TODO: remove
    int getAttributeValueType(int index);
    int getAttributeValueData(int index);
};