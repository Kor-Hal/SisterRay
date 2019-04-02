#include "primitives.h"

static void drawTextWidget(TextWidget* textWidget){
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
    widget->widget.xCoordinate = params.xCoordinate;
    widget->widget.yCoordinate = params.yCoordinate;
    widget->text = strdup(params.stringToDraw);
    widget->textColor = params.textColor;
    widget->priority = params.textThickness;
    return widget;
}

static void drawNumberWidget(NumberWidget* numberWidget) {
    gameDrawNumbers(
        numberWidget->widget.xCoordinate,
        numberWidget->widget.yCoordinate,
        numberWidget->number,
        numberWidget->numberColor,
        numberWidget->digitCount,
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

static void drawBoxWidget(BoxWidget* boxWidget) {
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

void addChildWidget(Widget* widget, TextWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}

void addChildWidget(Widget* widget, NumberWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}

void addChildWidget(Widget* widget, BoxWidget* child_widget, std::string name) {
    addChildWidget(widget, (Widget*)child_widget, name);
}


