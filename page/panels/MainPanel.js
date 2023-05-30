import { Panel } from "Panel";
import {
  TimeCountField,
} from "Field";

import { addTooltip } from "Tooltip";

import info from "Images/info.svg";
import gear from "Images/gear.svg";

export class MainPanel extends Panel {
  constructor() {
    // Always call super first in constructor
    super({
      checked: true,
    });

    this.name = "general";

    this.title = "Modbus Relay ATI";
    this.icon = info;
    this.enabled = true;

    const style = document.createElement("style");
    style.innerHTML = `
p {
  font-family: "Poppins";
  font-size: 14px;
}

p,
img {
    display: inline-block;
    vertical-align: middle;
}

img {
    width: 25px;
    height: 25px;
    margin-inline: 5px;
}`;
    this.wrapper.appendChild(style);

    const mainText = document.createElement("p");
    mainText.innerHTML = `Bem vindo a interface de configuração do Relay Modbus ATI! Clique no icone <img src='${gear}'/> para configurar o dispositivo`;
    this.wrapper.appendChild(mainText);

    new TimeCountField({
      inline: true,
      readonly: true,
      panel: this,
      name: "upTime",
      label: "Tempo ligado",
    });
  }
}

customElements.define("main-panel", MainPanel);
