import { Page } from "Page";

import { addStyleSheet } from "Base/utils";
import colors from "Styles/colors.css";
import fonts from "Styles/fonts.css";

import icon from "Images/favicon.svg";

import { MainPanel } from "./panels";

import { default as root } from "./proto.pb.js";

addStyleSheet(document.body, colors);
addStyleSheet(document.body, fonts);

document.title = "ATI - Relay";
var link = document.querySelector("link[rel~='icon']");
if (!link) {
    link = document.createElement('link');
    link.rel = 'icon';
    document.head.appendChild(link);
}
link.href = icon;
document.body.classList.add("light-mode");


// const header = new Header({ container: document.body });
const page = new Page({
  container: document.body,
  configFile: "/settings",
  configProto: root,
});

page.header.buttonLeft.style.opacity = 0;
page.header.buttonLeft.style.pointerEvents = "none";

page.header.buttonCenter.addEventListener("click", (e) => {
  location.href = "https://www.ati.com.br";
});

page.header.buttonRight.addEventListener("click", (e) => {
  location.href = "config";
});

const container = page.addContainer();

container.appendChild(new MainPanel());
