#include "SequenceJSLoader.h"
#include "Internationalization/Text.h"

FString SEQUENCE_JS_HTML()
{
  return R"""(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Sequence</title>
</head>

<body>
  <div>Initializing Sequence...</div>
  <form onsubmit="event.preventDefault(); exe();">
  <input type="text" id="textinput"></input>
  <input type="submit" id="mysubmit" value="Go">
  </form>
  <script>
    function appendLog(log, color) {
      const el = document.createElement('div');
      el.innerText = `${log}`;
      el.style.color = color;
      document.body.appendChild(el);
    }
    console.log = (...args) => {
      appendLog(args.join(' '), 'black')
    };
    console.warn = (...args) => {
      appendLog(args.join(' '), 'orange')
    };
    console.error = (...args) => {
      appendLog(args.join(' '), 'red')
    };
    console.log('Example log :)');
    console.warn('Example warn :)');
    console.error('Example err :)');

    function exe(ev) {
      try {
      eval(document.getElementById("textinput").value);
      } catch(e) {
        console.error("Eval failed: " + e.message);
      }
    }
  </script>
  <script>
    console.log("Loading sequence JS");
  </script>
  <script src="https://unpkg.com/0xsequence@latest/dist/0xsequence.umd.min.js"></script>
  <script>
    console.log("Loaded sequence JS!");
  </script>
  <script>
    console.log('Initializing wallet...');
    sequence.sequence.initWallet('polygon', { walletAppURL: 'http://192.168.2.84:3333/'});
    console.log('Wallet initialized.');
    sequence.sequence
      .getWallet()
      .connect({
        app: 'Unreal Game :)',
        authorize: true,
        settings: {
          theme: 'light',
          bannerUrl: 'https://sequence.xyz/built-in-security/animation.webp',
          includedPaymentProviders: ['moonpay', 'ramp'],
          defaultFundingCurrency: 'matic',
          lockFundingCurrencyToDefault: false
        }
      })
      .then(() => {
        console.log("Connected!");
        sequence.sequence
          .getWallet()
          .getSigner()
          .signMessage('Hello from unreal!');
      }).catch(console.error);
  </script>
</body>

</html>
)""";
}