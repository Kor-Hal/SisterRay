#include "primitives.h"
#include "../impl.h"
#include <exception>

void drawTextWidget(TextWidget* textWidget){
    gameDrawString(
        textWidget->widget.xCoordinate,
        textWidget->widget.yCoordinate,
        textWidget->text,
        textWidget->textColor,
        textWidget->priority
    );
}

TextWidget* createTextWidget(drawTextParams params, std::string name) {
    TextWidget* widget = (TextWidget*)createWidget(name, sizeof(TextWidget), &kTextWidgetClass);
    srLogWrite("text widget class: %p", &kTextWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->text = params.stringToDraw;
    widget->textColor = params.textColor;
    widget->priority = params.priority;
    return widget;
}

SISTERRAY_API void setTextParams(drawTextParams* params, i32 xCoordinate, i32 yCoordinate, char* stringToDraw, color textColor, float priority) {
    params->xCoordinate = xCoordinate;
    params->yCoordinate = yCoordinate;
    params->stringToDraw = stringToDraw;
    params->textColor = textColor;
    params->priority = priority;
}

bool isTextWidget(Widget* widget) {
    return (widget->klass == &kTextWidgetClass);
}

const WidgetClass* TextWidgetKlass() {
    return &kTextWidgetClass;
}

SISTERRAY_API void updateText(Widget* widgetToUpdate, char* text) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->text = text;
    }
    else {
        throw std::invalid_argument("attempting to update TextWidget text property of an invalid Widget type");
    }
}

SISTERRAY_API void updateTextColor(Widget* widgetToUpdate, color textColor) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->textColor = textColor;
    }
    else {
        throw std::invalid_argument("attempting to update TextWidget color property of an invalid Widget type");
    }
}

SISTERRAY_API void updateTextPriority(Widget* widgetToUpdate, float priority) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        typedPtr->priority = priority;
    }
    else {
        throw std::invalid_argument("attempting to update TextWidget priority property of an invalid Widget type");
    }
}

SISTERRAY_API char* getText(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        if(typedPtr->text)
            return typedPtr->text;
        return NULL
    }
    else {
        throw std::invalid_argument("attempting to fetch TextWidget text property of an invalid Widget type");
    }
}

SISTERRAY_API color getTextColor(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        return typedPtr->textColor;
    }
    else {
        throw std::invalid_argument("attempting to fetch TextWidget color property of an invalid Widget type");
    }
}

SISTERRAY_API float getTextPriority(Widget* widgetToUpdate) {
    if (isTextWidget(widgetToUpdate)) {
        auto typedPtr = (TextWidget*)widgetToUpdate;
        return typedPtr->priority;
    }
    else {
        throw std::invalid_argument("attempting to fetch TextWidget priority property of an invalid Widget type");
    }
}

//Begin Number Widget API Implementations
void drawNumberWidget(NumberWidget* numberWidget) {
    gameDrawNumbers(
        numberWidget->widget.xCoordinate,
        numberWidget->widget.yCoordinate,
        numberWidget->number,
        numberWidget->digitCount,
        numberWidget->numberColor,
        numberWidget->priority
    );
}

NumberWidget* createNumberWidget(drawNumberParams params, std::string name) {
    NumberWidget* widget = (NumberWidget*)createWidget(name, sizeof(NumberWidget), &kNumberWidgetClass);
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->number = params.numberToDraw;
    widget->numberColor = params.numberColor;
    widget->digitCount = params.charCount;
    widget->priority = params.numberThickness;
    return widget;
}

SISTERRAY_API void setNumberParams(drawNumberParams* params, i32 xCoord, i32 yCoord, u32 number, u8 numChars, color numColor, float priority) {
    params->xCoordinate = xCoord;
    params->yCoordinate = yCoord;
    params->numberToDraw = number;
    params->charCount = numChars;
    params->numberColor = numColor;
    params->numberThickness = priority;
}

bool isNumberWidget(Widget* widget) {
    return (widget->klass == &kNumberWidgetClass);
}

const WidgetClass* NumberWidgetKlass() {
    return &kNumberWidgetClass;
}

SISTERRAY_API void updateNumber(Widget* widgetToUpdate, u32 number) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->number = number;
    }
    else {
        throw std::invalid_argument("attempting to update NumberWidget number property of an invalid Widget type");
    }
}

SISTERRAY_API void updateNumberColor(Widget* widgetToUpdate, color numberColor) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->numberColor = numberColor;
    }
    else {
        throw std::invalid_argument("attempting to update NumberWidget color property of an invalid Widget type");
    }
}


SISTERRAY_API void updateNumberPriority(Widget* widgetToUpdate, float priority) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        typedPtr->priority = priority;
    }
    else {
        throw std::invalid_argument("attempting to update NumberWidget priority property of an invalid Widget type");
    }
}

SISTERRAY_API u32 getNumber(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->number
    }
    else {
        throw std::invalid_argument("attempting to fetch NumberWidget number property from an invalid Widget type");
    }
}

SISTERRAY_API color getNumberColor(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->numberColor;
    }
    else {

        throw std::invalid_argument("attempting to fetch NumberWidget color property from an invalid Widget type");
    }
}

SISTERRAY_API float getNumberPriority(Widget* widgetToUpdate) {
    if (isNumberWidget(widgetToUpdate)) {
        auto typedPtr = (NumberWidget*)widgetToUpdate;
        return typedPtr->priority;
    }
    else {
        throw std::invalid_argument("attempting to fetch NumberWidget priority property from an invalid Widget type");
    }
}

void drawBoxWidget(BoxWidget* boxWidget) {
    drawBoxParams params = {
        boxWidget->drawDistanceXa,
        boxWidget->drawDistanceXb,
        boxWidget->drawDistanceYa,
        boxWidget->drawDistanceYb
    };
    gameDrawBox((i16*)&params, boxWidget->priority);
}

BoxWidget* createBoxWidget(drawBoxParams params, std::string name) {
    BoxWidget* widget = (BoxWidget*)createWidget(name, sizeof(BoxWidget), &kBoxWidgetClass);
    widget->drawDistanceXa = params.drawDistance1;
    widget->drawDistanceXb = params.drawDistance2;
    widget->drawDistanceYa = params.drawDistance3;
    widget->drawDistanceYb = params.drawDistance4;
    widget->priority = params.boxFloat;
    return widget;
}


SISTERRAY_API void setBoxParams(drawBoxParams* params, i16 drawDistance1, i16 drawDistance2, u16 drawDistance3, u16 drawDistance4, float priority) {
    params->drawDistance1 = drawDistance1;
    params->drawDistance2 = drawDistance2;
    params->drawDistance3 = drawDistance3;
    params->drawDistance4 = drawDistance4;
    params->boxFloat = priority;
}

bool isBoxWidget(Widget* widget) {
    return (widget->klass == &kBoxWidgetClass);
}

const WidgetClass* BoxWidgetKlass() {
    return &kBoxWidgetClass;
}

SISTERRAY_API void resizeBox(Widget* widgetToUpdate, i16 drawDistance1, i16 drawDistance2, i16 drawDistance3, i16 drawdistance4) {
    if (isBoxWidget(widgetToUpdate)) {
        auto typedPtr = (BoxWidget*)widgetToUpdate;
        typedPtr->drawDistanceXa = drawDistance1;
        typedPtr->drawDistanceXb = drawDistance2;
        typedPtr->drawDistanceYa = drawDistance3;
        typedPtr->drawDistanceYb = drawdistance4;
    }
    else {
        throw std::invalid_argument("attempting to resize a widget which is not a Box Widget");
    }
}


