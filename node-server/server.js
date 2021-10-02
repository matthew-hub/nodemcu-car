const express = require("express");
const app = express();
const expressWs = require("express-ws")(app);

const path = require("path");

const port = 3000;

// this snippet gets the local ip of the node.js server. copy this ip to the client side code and add ':3000'
require("dns").lookup(require("os").hostname(), function (err, add, fam) {
  console.log("addr: " + add);
});

app.use(express.static(path.join(__dirname, "public")));

app.listen(process.env.PORT || port, () => {
  // eslint-disable-next-line no-console
  console.log(`Example app listening at http://localhost:${port}`);
});
