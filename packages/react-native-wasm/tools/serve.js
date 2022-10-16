const Koa = require('koa');
const koaStatic = require('koa-static');
const path = require('path');

module.exports = ({ reactNativeWasmDir }) => {
  const app = new Koa();

  app.use(async (ctx, next) => {
    ctx.set('access-control-allow-origin', '*');
    ctx.set('access-control-allow-headers', 'Origin, X-Requested-With, Content-Type, Accept');
    ctx.set('access-control-allow-methods', 'GET, HEAD, OPTIONS');
    ctx.set('cross-origin-opener-policy', 'same-origin');
    ctx.set('cross-origin-embedder-policy', 'require-corp');

    return next();
  });

  app.use(koaStatic(path.join(reactNativeWasmDir, 'dist')));

  app.listen(8080, () => {
    console.log('Serving files through 8080');
  });
};
