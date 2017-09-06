# ORC_log_parser

功能簡介: (1)由system log(dxpcon.log)篩選出所有已生產產品使用的對位參數。(2)依system log內紀錄生產時間與alignment settings(measure.ini)備存時間，挑選正確的備存檔。(3)合併system log與alignment settings內紀錄參數至單一表格(.csv)，並提供對位結果。

## 1.ORC曝光機 product log產生流程
### 1-1.Product log 製作流程圖
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/ORC%20exposurer.png?raw=true) 

很幸運地，與Fuji曝光機不同，ORC曝光機提供了所有生產參數的完整履歷。但不幸的是，ORC提供的生產履歷並未統一在同一份文件內；更不幸地，整理成良好格式的alignment log和product log都遺漏了太多項目，導致我們必須閱讀完整但龐雜的system log。

### 1-2.各模組紀錄參數
![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/ORC%20module%20members.png?raw=true)

先來看一下各模組紀錄的參數。由上圖可以看到alignment log和product log幾乎遺漏了所有job settings、alignment settings、light settings內設定的項目。另一方面system log則幾乎完整記錄了所有參數；除了alignment settings以外。還好system log至少提供了線索: alignment settings的檔名。因此蒐集所有system log與alignment settings的備存檔即可找出所有已生產產品使用的對位參數。

![picture alt](https://github.com/bn90207/ORC_log_parser/blob/master/illustrations/log_size.png?raw=true)

閱讀system log最大的困難在於其中包含太多無關的資訊。如上圖，在2017/2/6 05:01:55至2017/2/6 12:41:11期間，system log內紀錄的資料多達37萬行，但經過程式篩選出的生產數量僅有1054筆。這也是需要由程式來取代人員閱讀的主因。

## 2.Collector
### 2-1.Collector 執行流程圖
