/**
 * 日志输出测试
 * 期待结果：
 * ```
 * 开始运行
 * 运行结束
 * undefined sfs false undefined null [object Object] 1,2,3
 * ```
 */

log('开始运行');
setTimeout(x => log(x, "sfs", false, undefined, null, {}, [1, 2, 3]), 1000);
log('运行结束');