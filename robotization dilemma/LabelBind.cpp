#include "LabelBind.h"

template <typename ...Ts>
void LabelBind<Ts...>::render() {
  renderBitmapString((cax + cbx) / 2, (cay + ((align) ? cby : cay)) / 2, text.str(), textColor, align);
}

