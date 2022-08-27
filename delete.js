const fs = require('fs');
stat = fs.stat;
const path = require('path');
/**
 * 删除目录下 .DS_Store 文件方法
 * @param  String dir 文件夹名称
 */
let nods = function (dir) {
    fs.readdir(dir, function(err, files) {
        files.forEach(function(filename) {
            var src = path.join(dir, filename);
            stat(src, function (err, st) {
                if (err) { throw err; }
                // 判断是否为文件
                if (st.isFile()) {
                    if (/^\._.*$/.test(filename)) {
                        fs.unlink(src, (err) => {
                          if (err) throw err;
                          console.log('成功删除' + src);
                        });
                    }
                } else {
                    // 作为文件夹处理
                    nods(src);
                }
            });
        });
    });
};

nods('./');
