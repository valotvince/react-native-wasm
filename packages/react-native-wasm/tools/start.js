#!/usr/bin/env node

const build = require('./build');
const serve = require('./serve');

const start = async () => {
  await build();
  serve();
};

start();
