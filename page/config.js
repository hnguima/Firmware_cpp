import { Page } from "Page";
import { addStyleSheet } from "Base/utils";
import { addTooltip } from "Tooltip";

import { ChannelPanel } from "./panels";

// import * as Panels from "./panels";
import {
  LoginPanel,
  InfoPanel,
  FirmwarePanel,
  ConfigPanel,
  GeneralPanel,
  EthernetPanel,
  WifiPanel,
  ServerPanel,
} from "Panel";

import colors from "Styles/colors.css";
import fonts from "Styles/fonts.css";

import info from "Images/info.svg";
import web from "Images/web.svg";
import channel from "Images/channel.svg";
import tools from "Images/tools.svg";

import config from "./config.json";
import defaultConfig from "./default.json";

addStyleSheet(document.body, colors);
addStyleSheet(document.body, fonts);

document.body.classList.add("light-mode");

// const header = new Header({ container: document.body });
const page = new Page({
  container: document.body,
  configFile: defaultConfig,
});

page.header.buttonCenter.addEventListener("click", (e) => {
  location.href = "index.html";
});

const tabs = {
  info: {
    name: "Info",
    container: page.addContainer(),
    img: info,
    active: true,
  },
  net: {
    name: "Redes",
    container: page.addContainer(),
    img: web,
  },
  channel: {
    name: "Canais",
    container: page.addContainer(),
    img: channel,
  },
  tools: {
    name: "Ferramentas",
    container: page.addContainer(),
    img: tools,
  },
};

for (const tab in tabs) {
  page.sidebar.appendTab(
    tabs[tab].name,
    tabs[tab].img,
    tabs[tab].container,
    tabs[tab].active
  );
}

tabs["info"].container.appendChild(new InfoPanel());
tabs["info"].container.appendChild(new LoginPanel());

tabs["net"].container.appendChild(new EthernetPanel());
tabs["net"].container.appendChild(new WifiPanel());
tabs["net"].container.appendChild(new ServerPanel());

tabs["channel"].container.appendChild(
  new ChannelPanel({
    name: "channel-1",
    title: "Canal 1",
  })
);
tabs["channel"].container.appendChild(
  new ChannelPanel({
    name: "channel-2",
    title: "Canal 2",
  })
);

tabs["tools"].container.appendChild(new GeneralPanel());
tabs["tools"].container.appendChild(new FirmwarePanel());
tabs["tools"].container.appendChild(new ConfigPanel());

import { default as root } from "./proto.js";

// Exemplary payload
var payload = { wifi: { ssid: "asdfasdf", password: "testes" } };

// Verify the payload if necessary (i.e. when possibly incomplete or invalid)
var errMsg = root.MainConfig.verify(payload);
if (errMsg) throw Error(errMsg);

// Create a new message
var message = root.MainConfig.create(payload); // or use .fromObject if conversion is necessary
console.log(message);

// Encode a message to an Uint8Array (browser) or Buffer (node)
var buffer = root.MainConfig.encode(message).finish();
console.log(buffer);
// ... do something with buffer

// Decode an Uint8Array (browser) or Buffer (node) to a message
var message = root.MainConfig.decode(buffer);
console.log(message);
// ... do something with message

// If the application uses length-delimited buffers, there is also encodeDelimited and decodeDelimited.

// Maybe convert the message back to a plain object
var object = root.MainConfig.toObject(message, {
  longs: String,
  enums: String,
  bytes: String,
  // see ConversionOptions
});