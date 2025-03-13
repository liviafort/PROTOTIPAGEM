import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { Wifi } from './credentials/wifi-credentials.entity';
import { WifiController } from './credentials/credentials.controller';
import { join } from 'path';
import { ServeStaticModule } from '@nestjs/serve-static';

@Module({
  imports: [
    ServeStaticModule.forRoot({
      rootPath: join(__dirname, 'esp32'),
      serveRoot: '/esp32',
    }),
    TypeOrmModule.forRoot({
      type: 'postgres',
      url: process.env.DATABASE_URL,
      ssl: true,
      extra: { ssl: { rejectUnauthorized: false } },
      entities: [Wifi],
      synchronize: true,
    }),
    TypeOrmModule.forFeature([Wifi])
  ],
  controllers: [WifiController],
})
export class AppModule {}