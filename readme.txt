オープンソースフォトレタッチソフト Photo Operator


【特徴】
・無制限アンドゥ／リドゥ（メモリのみに依存）
・読み込み／保存できるファイル形式をプラグインで追加可能
・レタッチのためのフィルタをプラグインで追加可能
・標準的で扱いやすいインターフェース
・Windows XPのビジュアルスタイルに対応
・オープンソース


【インストール方法】


【アンインストール方法】


【ライセンス】
現在のところは、各種フォーマットのライブラリ以外は暫定的にGNU GPLに従うものとします。
しかし、将来的にはより自由なライセンスに変更する予定です。


【動作環境】
OS      : Microsoft Windows 95 / 98 / Me / NT4.0 / 2000 / XP
CPU     : Intel Pentium 166MHz以降
メモリ  : 32MB以上
HDD容量 : 10MB以上
その他  : Internet Explorer 4.0以降がインストールされていること


【動作確認環境（２台のマシンで実行確認）】
           | DELL Inspiron 8100                          | SONY VAIO PCG-GRX90/P
-----------+---------------------------------------------+------------------------------------------
OS         | Microsoft Windows 2000 Professional Edition | Microsoft Windows XP Professional Edition
CPU        | Intel Pentium IV 1.2GHz                     | Intel Pentium IV 1.7GHz
メモリ     | 512MB                                       | 512MB
HDD容量    | 24GB                                        | 40GB


【ディレクトリ構成】
POP
├─bin                                   リリースモードでコンパイルしたバイナリ＆設定バックアップ用バッチファイル
│  ├─help                                  各国語のヘルプファイル
│  ├─languages                             言語パックリソース
│  ├─pdf                                   プログラミングに関するドキュメント
│  └─plugins                               プラグインファイル
├─documents                             /POP/bin/pdf  の中にあるPDFファイルのTeXソース
├─hhp                                   /POP/bin/help の中にあるCHMファイルのHTMLソース
└─src                                   ソースコード
    ├─common                                Photo Operator本体やプラグインで共用されるコード
    ├─language                              言語パックのソースファイル
    ├─plugin                                プラグインのソースファイル
    └─POP                                   Photo Operator本体のソースファイル


【使用コンパイラ】
Microsoft Visual C++ 6.0 Professional Edition Service Pack 5 + HTML Help Workshop

※Windows 98以降、Windows NT 4.0 Servce Pack 4以降で、
 メニューやダイアログボックスにMS UI Gothicフォントを使用したい方は、
 設定→環境設定→言語カテゴリ で、"ResJpn(MS UI Gothic).dll"を選択してください。
 Windows 95でも使用したいという方は、
 http://www.microsoft.com/japan/win95/msgotupd/default.asp
 を参照してパッチをインストールしてください。


【未実装機能】
・クリップボード関係全般（選択、コピー、貼り付け等）


【実装不十分な機能】
・操作→減色（とりあえず作ってみたものなので、遅い＆汚い）
・設定→環境設定（細かいオプションが指定できない）
・英語リソース（中途半端に作ってる）
・ヘルプファイル（同上）
・インストーラ・アンインストーラの作成
　（設定は"HKEY_CURRENT_USER\Software\SHIMATARO\Photo Operator"に保存しているので、手動で消せる人はどうぞ）
・その他、細かいレタッチ機能（色調補正とかレイヤーとか…）


まだまだたくさん機能を追加する予定です…
