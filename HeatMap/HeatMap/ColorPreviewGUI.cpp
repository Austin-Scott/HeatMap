#include "ColorPreviewGUI.h"

ColorPreviewGUI::ColorPreviewGUI(form & frm) : panel<false>(frm)
{
	d = new drawing(frm);
	d->draw([&](paint::graphics& graph) {
		graph.rectangle(rectangle(pos().x, pos().y, size().width, size().height), true, nana::color(c.getR(), c.getG(), c.getB(), (double)c.getA()/255.0));
	});
}

ColorPreviewGUI::~ColorPreviewGUI()
{
	delete d;
}

void ColorPreviewGUI::setColor(Color init)
{
	c = init;
	d->update();
}
