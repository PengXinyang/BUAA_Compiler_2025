//
// Created by PengXinyang on 24-12-10.
//

#ifndef CONFIGOPTIMIZER_H
#define CONFIGOPTIMIZER_H

/**
* 优化开关配置，方便输出优化前后的代码
*/
class ConfigOptimizer {
private:
    //全局优化开关，如果禁用将不进行任何优化
    static bool config_optimize;
public:
    /**
    * @return 返回是否进行全局的优化
    */
    static bool isConfigOptimize();

    /**
    * 设置优化的开关
    * @param is_optimize
    */
    static void setConfigOptimize(bool is_optimize);
    /**
    * 初始化优化配置器
    */
    static void initConfigOptimizer();
};



#endif //CONFIGOPTIMIZER_H
