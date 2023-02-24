import { SwitchPanel } from "Panel";
import { SelectField, IntegerField, FileField, ButtonField } from "Field";
import { addTooltip } from "Tooltip";

import icon from "Images/channel.svg";
import gear from "Images/gear.svg";
import download from "Images/download.svg";
import factory from "Images/factory.svg";

export class ChannelPanel extends SwitchPanel {
  constructor(options) {
    // Always call super first in constructor
    super(options);

    this.name = (options && options.name) || "";
    this.title = (options && options.title) || "";

    this.icon = icon;
    this.enabled = true;

    new SelectField({
      panel: this,
      name: "type",
      label: "Modo de operação",
      options: [
        {
          text: "ModBUS RTU",
        },
        {
          text: "Anygrid",
        },
      ],
    });

    const UARTheader = document.createElement("h3");
    UARTheader.innerHTML = "Configurações do UART";
    this.wrapper.appendChild(UARTheader);

    new IntegerField({
      panel: this,
      name: "baudrate",
      label: "Baud Rate",
      max: 65535,
    });

    new SelectField({
      panel: this,
      name: "parity",
      label: "Paridade",
      options: [
        {
          value: 0,
          text: "Nenhuma",
        },
        {
          value: 2,
          text: "Par",
        },
        {
          value: 3,
          text: "Ímpar",
        },
      ],
    });

    new SelectField({
      panel: this,
      name: "stop-bits",
      label: "Quantidade de Stop Bits",
      options: [
        {
          value: 1,
          text: "1",
        },
        {
          value: 2,
          text: "1.5",
        },
        {
          value: 3,
          text: "2",
        },
      ],
    });

    new SelectField({
      panel: this,
      name: "data-bits",
      label: "Quantidade de bits de dado (Data Bits)",
      options: [
        {
          text: "5",
        },
        {
          text: "6",
        },
        {
          text: "7",
        },
        {
          text: "8",
        },
      ],
    });

    const TCPheader = document.createElement("h3");
    TCPheader.innerHTML = "Configurações do TCP";
    this.wrapper.appendChild(TCPheader);

    new IntegerField({
      panel: this,
      name: "tcp-port",
      placeholder: "port",
      label: "Porta TCP",
      max: 65535,
    });

    new IntegerField({
      panel: this,
      name: "timeout",
      placeholder: "ms",
      label: "Timeout (ms)",
      max: 65535,
    });
  }
}

customElements.define("channel-panel", ChannelPanel);
